// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewCommandTable.h"
#include "Kismet/GameplayStatics.h"
#include "../MyNewGameInstance.h"

// Sets default values for this component's properties
UMyNewCommandTable::UMyNewCommandTable()
{
	CurrentWeapon = ENewWeaponType::E_NONE;
	CurrentCommandName = ENewCommandName::E_BASE;
	IsInitCompleted = false;
}


// Called when the game starts
void UMyNewCommandTable::BeginPlay()
{
	Super::BeginPlay();
}

void UMyNewCommandTable::LoadCommandTable() {
	if (IsInitCompleted == true) return;

	UDataTable* TmpTable = Cast<UMyNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetTotalCommandTable();

	if (IsValid(TmpTable)) {
		TArray<FName> RowNames = TmpTable->GetRowNames();

		for (auto name : RowNames)
		{
			static const FString ContextString(TEXT("Set CommandTable"));

			auto CommandTable = TmpTable->FindRow<FNewTotalCommandTable>(name, ContextString, true)->CommandTable;
			auto type = TmpTable->FindRow<FNewTotalCommandTable>(name, ContextString, true)->weapon;

			TMap<ENewCommandName, FNewChainAction> Commands;

			for (auto rowName : CommandTable->GetRowNames())
			{
				auto TempActionChain = CommandTable->FindRow<FNewChainActionTable>(rowName, ContextString, true)->Action;

				Commands.Add(TempActionChain.AttackType, TempActionChain);
			}
			TotalCommands.Add(type, Commands);
		}
	}
	IsInitCompleted = true;
}

void UMyNewCommandTable::ChangeCommandTable(const ENewWeaponType weapon, const ENewCommandName InitCommandName)
{
	if (CurrentWeapon == weapon) return;
	CurrentWeapon = weapon;

	switch (CurrentWeapon) {
	case ENewWeaponType::E_DUAL:
		CurrentCommands = &TotalCommands[ENewWeaponType::E_DUAL];
		break;
	case ENewWeaponType::E_KATANA:
		CurrentCommands = &TotalCommands[ENewWeaponType::E_KATANA];
		break;
	case ENewWeaponType::E_AXE:
		CurrentCommands = &TotalCommands[ENewWeaponType::E_AXE];
		break;
	}
	EnableAction = FindEnableAction(InitCommandName);
	SetCurrentCommandName(InitCommandName);
}

void UMyNewCommandTable::SetCurrentCommandName(const ENewCommandName commandName)
{
	CurrentCommandName = commandName;
	EnableAction = &(CurrentCommands->Find(CurrentCommandName)->EnableChainAction);
	Notify();
}

TArray<FNewChainAction> UMyNewCommandTable::MakeEnableChainAction() {
	TArray<FNewChainAction> Result;
	if (EnableAction->Num() <= 0) return Result;
	for (auto action : *EnableAction) {
		auto TempEnable = CurrentCommands->Find(action);
		if (TempEnable != nullptr) {
			Result.Add(*TempEnable);
		}
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