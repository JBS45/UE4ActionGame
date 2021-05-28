// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewInputBuffer.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "MyNewCommandTable.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "../PlayerCharacter/MyNewCharacterAnimInstance.h"
#include "../PlayerCharacter/MyNewPlayerController.h"
#include "../PlayerCharacterComponents/CharacterStatusComponent.h"

// Sets default values for this component's properties
UMyNewInputBuffer::UMyNewInputBuffer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentInputBufferState = EInputBufferState:: E_DISABLE;
	CommandTable = CreateDefaultSubobject<UMyNewCommandTable>(TEXT("CommandTable"));
	CurrentCommandName = ENewCommandName::E_BASE;
	ResetCommandName = ENewCommandName::E_BASE;
	CurrentWeaponType = ENewWeaponType::E_NONE;
	TimerFlag = false;
	IsSprintFlag = false;
}


// Called when the game starts
void UMyNewInputBuffer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyNewInputBuffer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SaveMoveKey();
	InputBufferStateProcess(DeltaTime);
}

void UMyNewInputBuffer::InputBufferStateChange(EInputBufferState state) {
	CurrentInputBufferState = state;
}
void UMyNewInputBuffer::InputBufferStateProcess(float detla) {
	switch (CurrentInputBufferState) {
	case EInputBufferState::E_ENABLE:
		CheckAction(detla);
		break;
	case EInputBufferState::E_ONLYMOVE:
		break;
	case EInputBufferState::E_ONLYACTION:
		CheckAction(detla);
		break;
	case EInputBufferState::E_DISABLE:
		break;
	}
}
void UMyNewInputBuffer::MoveForward(float value)
{
	if (value >= 0.1f) {
		SaveKeyState.KeyFlag |= (int32)EMoveKeyFlag::E_FORWARD;
	}
	else if (value <= -0.1f) {
		SaveKeyState.KeyFlag |= (int32)EMoveKeyFlag::E_BACKWARD;
	}

	if (CurrentInputBufferState == EInputBufferState::E_ONLYACTION
		|| CurrentInputBufferState == EInputBufferState::E_DISABLE
		|| !IsValid(CurrentCharacter)) return;

	CurrentCharacter->MoveForward(value);

}
void UMyNewInputBuffer::MoveRight(float value)
{
	if (value >= 0.1f) {
		SaveKeyState.KeyFlag |= (int32)EMoveKeyFlag::E_RIGHT;
	}
	else if (value <= -0.1f) {
		SaveKeyState.KeyFlag |= (int32)EMoveKeyFlag::E_LEFT;
	}

	if (CurrentInputBufferState == EInputBufferState::E_ONLYACTION
		|| CurrentInputBufferState == EInputBufferState::E_DISABLE
		|| !IsValid(CurrentCharacter)) return;

	CurrentCharacter->MoveRight(value);

}
void UMyNewInputBuffer::CameraLookUp(float value)
{
	if (!IsValid(CurrentCharacter)) return;

	CurrentCharacter->CameraLookUp(value);
}
void UMyNewInputBuffer::CameraTurn(float value)
{
	if (!IsValid(CurrentCharacter)) return;

	CurrentCharacter->CameraTurn(value);
}

void UMyNewInputBuffer::PressShift() {
	if (IsValid(CurrentCharacter)) {
		ActionInputBuffer.Add(Click(ENewActionKey::E_SHIFT));
		if(CurrentCharacter->GetCurrentActionState() == ENewActionState::E_NONE
			&&CurrentCharacter->GetCurrentPlayerState() == ENewPlayerState::E_IDLE){
			CurrentController->ChangePlayerState.Broadcast(ENewPlayerState::E_SPRINT);
		}
		IsSprintFlag = true;
	}
}
void UMyNewInputBuffer::ReleaseShift() {
	if (IsValid(CurrentCharacter)) {
		if (CurrentCharacter->GetCurrentPlayerState() == ENewPlayerState::E_SPRINT) {
			CurrentController->ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);
			IsSprintFlag = false;
		}
	}
}

FActionKeyState UMyNewInputBuffer::Click(const ENewActionKey key) {
	FActionKeyState Click;
	Click.Key = key;
	Click.TimeStamp = GetWorld()->TimeSeconds;
	return Click;
}


void UMyNewInputBuffer::SaveMoveKey() {
	SaveKeyState.TimeStamp = GetWorld()->TimeSeconds;
	FMoveKeyState TempKeyState;
	TempKeyState.KeyFlag = SaveKeyState.KeyFlag;
	TempKeyState.TimeStamp = SaveKeyState.TimeStamp;

	if (MoveInputBuffer.Num() <= 30) {
		MoveInputBuffer.Add(TempKeyState);
	}
	else {
		MoveInputBuffer.RemoveAt(0);
		MoveInputBuffer.Add(TempKeyState);
	}
	SaveKeyState.KeyFlag = (int32)EMoveKeyFlag::E_NONE;
}

void UMyNewInputBuffer::SetCurrentCharacter(AMyNewCharacter* character) {
	CurrentCharacter = character;
	CommandTable->LoadCommandTable();
	InputBufferStateChange(EInputBufferState::E_ENABLE);
	CurrentCommandName = ENewCommandName::E_BASE;
	ResetCommandName = ENewCommandName::E_BASE;
	ChangeWeaponState(ENewWeaponType::E_DUAL);
	CommandTable->Attach(this);

}

void UMyNewInputBuffer::ClearCurrentCharacter() {
	CurrentCharacter = nullptr;
	InputBufferStateChange(EInputBufferState::E_DISABLE);
}

void UMyNewInputBuffer::CheckAction(float delta) {
	if (CurrentActionState == ENewActionState::E_DOWN && ActionInputBuffer.Num() >= 1) {
		CurrentController->ChangeActionState.Broadcast(ENewActionState::E_GETUP);
		ActionInputBuffer.Empty();
		return;
	}

	if (TimerFlag == false && ActionInputBuffer.Num()>=1) {
		LastBeforeAction = MoveInputBuffer[MoveInputBuffer.Num() - 1];
		CheckTimer = ActionInputBuffer[0].TimeStamp + BufferDelay;
		TimerFlag = true;
	}
	if (TimerFlag==true&&(CheckTimer <= GetWorld()->TimeSeconds)) {
		ActiveCommand();
		TimerFlag = false;
		ActionInputBuffer.Empty();
		MoveInputBuffer.Empty();
	}
}
void UMyNewInputBuffer::ActiveCommand() {
	bool LeftFlag = false;
	bool RightFlag = false;

	TArray<FNewChainAction> CandidateAction;

	TArray<ENewActionKey> NoDuplicateActionInput;

	for (int i = 0; i < ActionInputBuffer.Num();) {
		auto key = ActionInputBuffer[i].Key;
		NoDuplicateActionInput.Add(key);
		ActionInputBuffer.RemoveAll([key](const FActionKeyState& rhs) {return rhs.Key == key; });
	}

	LeftFlag = NoDuplicateActionInput.ContainsByPredicate([](const ENewActionKey& key) {return (key == ENewActionKey::E_LEFTCLICK); });
	RightFlag = NoDuplicateActionInput.ContainsByPredicate([](const ENewActionKey& key) {return (key == ENewActionKey::E_RIGHTCLICK); });

	if (LeftFlag&&RightFlag) {
		NoDuplicateActionInput.Add(ENewActionKey::E_LEFTRIGHTCLICK);
	}

	TArray<FNewChainAction> ActionCheck;

	for (auto Input : NoDuplicateActionInput) {
		for (auto action : EnableActions) {
			if (action.ActionCommand == Input) {
				ActionCheck.Add(action);
			}
		}
	}

	for (auto element : ActionCheck) {
		if (element.MoveCommand == 0 || element.MoveCommand&LastBeforeAction.KeyFlag) {
			CandidateAction.Add(element);
		}
	}

	if (CandidateAction.Num() <= 0) return;

	CandidateAction.Sort([](const FNewChainAction& lhs, const FNewChainAction& rhs) { return lhs.Priority >= rhs.Priority; });

	//PlayAnimation
	ActivateCommand(CandidateAction[0].AttackType);
	
}

void UMyNewInputBuffer::ActivateCommand(ENewCommandName CommandName) {
	auto Montage = CommandTable->FindAnimation(CommandName);
	if (Montage == nullptr) {
		if (CommandName == ENewCommandName::E_EXCHANGE) {
			CurrentController->ChangeWeaponType.Broadcast(GetNextWeapon());
		}
	}
	else {
		if (CommandName == ENewCommandName::E_EVADE){
			bool result = CurrentCharacter->GetStatusManager()->UseStamina(15.0f);
			if (result == false) return;
		}
		if (CommandName == ENewCommandName::E_LEFTSTEP
		|| CommandName == ENewCommandName::E_RIGHTSTEP) {
			bool result = CurrentCharacter->GetStatusManager()->UseStamina(15.0f);
			CurrentController->LookCameraForwardDirection();
			if (result == false) return;
		}
		if (CommandName == ENewCommandName::E_POTION
			|| CommandName == ENewCommandName::E_PUTUPPOTION) {
			bool result = CurrentController ->GetPotionCanUse();
			if (result == false) return;
		}
		PlayAnimation(Montage);
		CommandTable->SetCurrentCommandName(CommandName);
		CurrentCommandName = CommandName;
	}
	
}
void UMyNewInputBuffer::PlayAnimation(UAnimMontage* montage) {
	auto AnimInst = CurrentCharacter->GetAnimInst();
	if (IsValid(AnimInst)) {
		AnimInst->PlayAnimMontage(montage);
	}
}

ENewWeaponType UMyNewInputBuffer::GetNextWeapon() {
	switch (CurrentWeaponType) {
	case ENewWeaponType::E_DUAL:
		return  ENewWeaponType::E_KATANA;
	case ENewWeaponType::E_KATANA:
		return  ENewWeaponType::E_AXE;
	case ENewWeaponType::E_AXE:
		return  ENewWeaponType::E_DUAL;
	default:
		return  ENewWeaponType::E_DUAL;
	}
}
void UMyNewInputBuffer::ChangePlayerState(const ENewPlayerState state) {
	if (CurrentPlayerState == state) return;
	CurrentPlayerState = state;
	switch (CurrentPlayerState) {
	case ENewPlayerState::E_IDLE:
		ResetCommandName = ENewCommandName::E_BASE;
		CommandTable->SetCurrentCommandName(ResetCommandName);
		CurrentCommandName = ResetCommandName;
		break;
	case ENewPlayerState::E_BATTLE:
		ResetCommandName = ENewCommandName::E_DRAW;
		break;
	case ENewPlayerState::E_SPRINT:
		ResetCommandName = ENewCommandName::E_BASE;
		CommandTable->SetCurrentCommandName(ResetCommandName);
		CurrentCommandName = ResetCommandName;
		break;
	case ENewPlayerState::E_DEAD:
		InputBufferStateChange(EInputBufferState::E_DISABLE);
		CommandTable->ChangeCommandTable(ENewWeaponType::E_DUAL);
		break;
	}
}
void UMyNewInputBuffer::ChangeActionState(const ENewActionState state) {
	if (CurrentActionState == state) return;
	CurrentActionState = state;
	switch (CurrentActionState)
	{
	case ENewActionState::E_NONE:
		InputBufferStateChange(EInputBufferState::E_ENABLE);
		break;
	case ENewActionState::E_ACTION:
	case ENewActionState::E_HIT:
	case ENewActionState::E_KNOCKBACK:
	case ENewActionState::E_GETUP:
		InputBufferStateChange(EInputBufferState::E_DISABLE);
		break;
	case ENewActionState::E_DOWN:
		InputBufferStateChange(EInputBufferState::E_ONLYACTION);
		break;
	}

}
void UMyNewInputBuffer::ChangeWeaponState(const ENewWeaponType weapon) {
	if (CurrentWeaponType == weapon) return;
	CurrentWeaponType = weapon;
	if (CurrentPlayerState == ENewPlayerState::E_IDLE) {
		CommandTable->ChangeCommandTable(CurrentWeaponType);
	}
	else if (CurrentPlayerState == ENewPlayerState::E_BATTLE) {
		CommandTable->ChangeCommandTable(CurrentWeaponType, ENewCommandName::E_DRAW);
	}
}

void UMyNewInputBuffer::NotifyReset() {
	CurrentCommandName = ResetCommandName;
	CommandTable->SetCurrentCommandName(ResetCommandName);
	MoveInputBuffer.Empty();
	ActionInputBuffer.Empty();
}
void UMyNewInputBuffer::NotifyCommand(TArray<FNewChainAction> actions) {
	EnableActions.Empty();
	EnableActions.Append(actions);
}
