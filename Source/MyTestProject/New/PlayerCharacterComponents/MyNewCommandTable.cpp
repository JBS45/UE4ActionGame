// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewCommandTable.h"
#include "Kismet/GameplayStatics.h"
#include "../MyNewGameInstance.h"
#include "../../MyTestProject.h"


// Sets default values for this component's properties
UMyNewCommandTable::UMyNewCommandTable()
{
	CurrentWeapon = ENewWeaponType::E_NONE;
	CurrentCommandName = ENewCommandName::E_BASE;
	IsInitCompleted = false;
	CheckName = "None";
}


// Called when the game starts
void UMyNewCommandTable::BeginPlay()
{
	Super::BeginPlay();
}

void UMyNewCommandTable::LoadCommandTable() {
	if (IsInitCompleted == true) return;

	auto GameInst = Cast<UMyNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	UDataTable* TmpTable = &GameInst->GetTotalCommandTable();

	if (IsValid(TmpTable)) {
		TArray<FName> RowNames = TmpTable->GetRowNames();

		for (auto name : RowNames)
		{
			static const FString ContextString(TEXT("Set CommandTable"));

			auto CommandTable = TmpTable->FindRow<FNewTotalCommandTable>(name, ContextString, true)->CommandTable;
			auto type = TmpTable->FindRow<FNewTotalCommandTable>(name, ContextString, true)->weapon;

			TMap<ENewCommandName,FNewChainAction> CommandMap;

			for (auto rowName : CommandTable->GetRowNames())
			{
				auto TempActionChain = CommandTable->FindRow<FNewChainActionTable>(rowName, ContextString, true)->Action;

				CommandMap.Add(TempActionChain.AttackType, TempActionChain);
			}
			FCommands TempCommandStruct = FCommands();
			TempCommandStruct.Chain = CommandMap;
			TotalCommands.Add(type, TempCommandStruct);
		}
	}
	IsInitCompleted = true;
}

void UMyNewCommandTable::ChangeCommandTable(const ENewWeaponType weapon, const ENewCommandName InitCommandName)
{
	if (CurrentWeapon == weapon) return;
	CurrentWeapon = weapon;

	if (CurrentCommands.Chain.Num()>0) {
		CurrentCommands.Chain.Empty();
	}
	if (CurrentWeapon == ENewWeaponType::E_NONE) {
		CurrentCommands = TotalCommands[ENewWeaponType::E_DUAL];
	}
	CurrentCommands = TotalCommands[CurrentWeapon];
	SetCurrentCommandName(InitCommandName);
}

void UMyNewCommandTable::SetCurrentCommandName(const ENewCommandName commandName)
{
	CurrentCommandName = commandName;
	Check();
	FindEnableAction(commandName);
	Notify();
}

TArray<FNewChainAction> UMyNewCommandTable::MakeEnableChainAction() {
	TArray<FNewChainAction> Result;
	if (EnableAction.Num() <= 0) return Result;
	for (auto action : EnableAction) {
		FNewChainAction TempEnable = CurrentCommands.Chain[action];
		Result.Add(TempEnable);

	}
	return Result;
}

void UMyNewCommandTable::Attach(IChangeCommand* Observer) {
	Observers.Add(Observer);
	Notify();
}

void UMyNewCommandTable::Notify() {
	auto result = MakeEnableChainAction();

	if (result.Num() <= 0) return;

	for (int i=0;i<Observers.Num();++i){
		if (Observers[i] == nullptr) {
			Observers.RemoveAt(i);
			--i;
			continue;
		}
		Observers[i]->NotifyCommand(result);
	}
}
UAnimMontage* UMyNewCommandTable::TestFindAnimation(const ENewCommandName actionName){
	ENewCommandName Temp = TestActionName;
	UAnimMontage* AnimMonatge = (CurrentCommands.Chain[actionName].ActionMontage);
	if (AnimMonatge == nullptr) return nullptr;
	FName MontageName = FName::FName(AnimMonatge->GetName());
	if (MontageName == "None") {
		TESTLOG(Error, TEXT("Montage Is Not Null, But Is not Valid, CommandName : %d"), Temp);
		return nullptr;
	}
	return AnimMonatge;
}

TArray<ENewCommandName>* UMyNewCommandTable::FindEnableAction(const ENewCommandName currentActionName){
	if (EnableAction.Num()>0) {
		EnableAction.Empty();
	}
	EnableAction = CurrentCommands.Chain[currentActionName].EnableChainAction;
	return &EnableAction;
};

void UMyNewCommandTable::Check() {
	if (CurrentCommands.Chain.Num() <= 0) return;
	for (auto command : CurrentCommands.Chain) {
		if (command.Value.ActionMontage != nullptr) {
			FName MonatageName = FName::FName(command.Value.ActionMontage->GetName());
			if (MonatageName == CheckName) {
				CurrentCommands = TotalCommands[CurrentWeapon];
				return;
			}
		}
	}
}