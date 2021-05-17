// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferManager.h"
#include "CommandTableManager.h"
#include "../Player/BaseCharacter.h"
#include "../Player/BasePlayerController.h"

// Sets default values for this component's properties
UInputBufferManager::UInputBufferManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	CurrentWeapon = EWeaponType::E_NOWEAPON;
	CurrentActionName = ECommandName::E_BASE;
	ResetActionName = ECommandName::E_BASE;
	StateSpecial = false;
}


// Called when the game starts
void UInputBufferManager::BeginPlay()
{
	Super::BeginPlay();


	//CommandTable->ActiveActionDel.AddUObject(this, &UInputBufferManager::SetCurrentAction);
	// ...
	bPressedW = false;
	bPressedS = false;
	bPressedD = false;
	bPressedA = false;
}


// Called every frame
void UInputBufferManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CharacterStateTick();

}

void UInputBufferManager::AddInputToInputBuffer(FMoveInputInfo input)
{
	if (!CanBufferInput) return;

	switch (input.InputType) {
	case EMoveKey::E_FORWARD:
		MoveAddInput(bPressedW, input);
		break;
	case EMoveKey::E_BACKWARD:
		MoveAddInput(bPressedS, input);
		break;
	case EMoveKey::E_RIGHT:
		MoveAddInput(bPressedD, input);
		break;
	case EMoveKey::E_LEFT:
		MoveAddInput(bPressedA, input);
		break;
	}

}
void UInputBufferManager::MoveAddInput(bool Pressed, FMoveInputInfo input)
{
	if (MoveInputBuffer.Num() <= 0)
	{
		MoveInputBuffer.Add(input);
	}
	else {

		if (Pressed) {
			int index = MoveInputBuffer.FindLastByPredicate([input](const FMoveInputInfo Lhs) {return Lhs.InputType == input.InputType; });
			if (index != INDEX_NONE) {
				MoveInputBuffer[index].TimeStamp = input.TimeStamp;
			}
		}
		else {
			if (MoveInputBuffer.Num() > 5) {
				MoveInputBuffer.RemoveAt(0);
			}
			MoveInputBuffer.Add(input);
		}
	}
}

void UInputBufferManager::AddInputToInputBuffer(FActionInputInfo input)
{
	if (!CanBufferInput) return;

	if (input.InputType == EActionKey::E_EVADE) {
		EvadeOrStep();
		return;
	}
	if (ActionInputBuffer.Num() == 0) {
		BufferCheckTime = input.TimeStamp;
	}
	if (ActionInputBuffer.ContainsByPredicate([input](const FActionInputInfo Lhs) {return Lhs.InputType == input.InputType; })) {
		auto Pressing = ActionInputBuffer.FindByPredicate([input](const FActionInputInfo Lhs) {return Lhs.InputType == input.InputType; });
		Pressing->TimeStamp = input.TimeStamp;
		return;
	}
	if (ActionInputBuffer.Num() > 5) {
		ActionInputBuffer.RemoveAt(0);
	}
	ActionInputBuffer.Add(input);;
}

void UInputBufferManager::ChainReset() {

	ReadyForNextAction(ResetActionName);
	CanBufferInput = true;
}

void UInputBufferManager::ReadyForNextAction(ECommandName newActionName) {
	CurrentActionName = newActionName;
	CommandTable->SetCurrentCommandName(newActionName);
}

void UInputBufferManager::PutUpWeapon() {
	if (CurrentState == ECharacterState::E_BATTLE) {
		PlayAnimation(ECommandName::E_PUTUP);
	}
}


void UInputBufferManager::ActionBufferCheck()
{

	if (ActionInputBuffer.Num() <= 0) return;

	if (BufferCheckTime + INPUT_BUFFER_DELAY <= GetWorld()->GetTimeSeconds()) {
		CheckCommand();
	}
	for (int i = 0; i < ActionInputBuffer.Num();) {
		if (ActionInputBuffer[i].TimeStamp + INPUT_BUFFER_DELAY <= GetWorld()->GetTimeSeconds()) {
			ActionInputBuffer.RemoveAt(i);
			continue;
		}
		i++;
	}
}
void UInputBufferManager::MoveBufferCheck() {
	for (int i = 0; i < MoveInputBuffer.Num();) {
		if (MoveInputBuffer[i].TimeStamp + INPUT_BUFFER_DELAY <= GetWorld()->GetTimeSeconds()) {
			MoveInputBuffer.RemoveAt(i);
			continue;
		}
		i++;
	}
}
bool UInputBufferManager::CheckCommand() {
	auto EnableActionName = CommandTable->FindEnableAction(CurrentActionName);
	BufferCheckTime = 0.0f;

	TMap<ECommnadPriority, ECommandName> CandidateAction;

	for (auto ActionName : EnableActionName)
	{
		FChainAction AciontInfo = CommandTable->FindAction(ActionName);
		TArray<EActionKey> ActionCommands = AciontInfo.ActionCommand;
		TArray<EMoveKey> MoveCommands = AciontInfo.MoveCommand;

		float MoveTimer = 0.0f;
		float ActionTimer = 0.0f;

		bool ValidMove = true;
		if (MoveCommands.Num() > 0) {
			ValidMove = MoveCommnadCheck(MoveCommands, MoveTimer);
		}
		bool ValidAction = ActionCommnadCheck(ActionCommands, ActionTimer);

		if (ValidMove && ValidAction) {
			CandidateAction.Add(AciontInfo.Priority, AciontInfo.AttackType);
		}
	}
	if (CandidateAction.Contains(ECommnadPriority::E_HIGH)) {
		auto actionName = CandidateAction.Find(ECommnadPriority::E_HIGH);
		PlayAnimation(*actionName);
		ClearInputBuffer();
		return true;
	}
	else if (CandidateAction.Contains(ECommnadPriority::E_NORMAL)) {
		auto actionName = CandidateAction.Find(ECommnadPriority::E_NORMAL);
		PlayAnimation(*actionName);
		ClearInputBuffer();
		return true;
	}
	else if (CandidateAction.Contains(ECommnadPriority::E_LOW)) {
		auto actionName = CandidateAction.Find(ECommnadPriority::E_LOW);
		PlayAnimation(*actionName);
		ClearInputBuffer();
		return true;
	}
	else {
		ClearInputBuffer();
		return false;
	}


	return true;
}

bool UInputBufferManager::MoveCommnadCheck(TArray<EMoveKey> Command, float& LastInput)
{

	LastInput = MAX_flt;
	if (MoveInputBuffer.Num() <= 0) return false;

	if (Command.Num() == 0) {
		return true;
	}

	if (Command.Num() == 1) {

		int last = MoveInputBuffer.Num() - 1;

		if (Command[0] == EMoveKey::E_ALLMOVE
			|| (Command[0] == MoveInputBuffer[last].InputType)) {
			LastInput = MoveInputBuffer[last].TimeStamp;
			return true;
		}
	}

	if (Command.Num() == 2) {
		int last = MoveInputBuffer.Num() - 1;

		if (Command[0] == MoveInputBuffer[last - 1].InputType
			&& Command[1] == MoveInputBuffer[last].InputType
			&& (MoveInputBuffer[last].TimeStamp - MoveInputBuffer[last - 1].TimeStamp) <= INPUT_BUFFER_DELAY) {
			LastInput = MoveInputBuffer[last].TimeStamp;
			return true;
		}
	}
	return false;

}
bool UInputBufferManager::ActionCommnadCheck(TArray<EActionKey> Command, float& FirstInput)
{
	FirstInput = MIN_flt;

	if (Command.Num() == 1 && ActionInputBuffer.Num() >= 1) {
		int last = ActionInputBuffer.Num() - 1;

		if ((Command[0] == ActionInputBuffer[0].InputType)) {
			FirstInput = ActionInputBuffer[last].TimeStamp;
			return true;
		}
	}

	if (Command.Num() == 2 && ActionInputBuffer.Num() >= 2) {
		int last = ActionInputBuffer.Num() - 1;

		if ((Command[0] == ActionInputBuffer[last - 1].InputType)
			&& (Command[1] == ActionInputBuffer[last].InputType)
			|| ((Command[1] == ActionInputBuffer[last - 1].InputType)
				&& (Command[0] == ActionInputBuffer[last].InputType)))
		{
			if ((ActionInputBuffer[last].TimeStamp - ActionInputBuffer[last - 1].TimeStamp) <= INPUT_BUFFER_DELAY) {
				FirstInput = ActionInputBuffer[last].TimeStamp;
				return true;
			}
		}
	}

	return false;
}

void UInputBufferManager::ChangeCharacterState(ECharacterState state) {
	if (CurrentState == state) return;
	CurrentState = state;
	switch (CurrentState) {
	case ECharacterState::E_IDLE:
		if (StateSpecial) {
			ResetActionName = ECommandName::E_SPECIAL;
		}
		else {
			ResetActionName = ECommandName::E_BASE;
		}
		CanBufferInput = true;
		break;
	case ECharacterState::E_BATTLE:
		if (StateSpecial) {
			ResetActionName = ECommandName::E_SPECIALDRAW;
		}
		else {
			ResetActionName = ECommandName::E_DRAW;
		}
		CanBufferInput = true;
		break;
	case ECharacterState::E_HIT:
		CanBufferInput = false;
		break;
	case ECharacterState::E_DOWN:
		CanBufferInput = false;
		break;
	case ECharacterState::E_DEAD:
		CanBufferInput = false;
		PrimaryComponentTick.bCanEverTick = false;
		break;
	}
}
void UInputBufferManager::CharacterStateTick() {
	switch (CurrentState) {
	case ECharacterState::E_IDLE:
	case ECharacterState::E_BATTLE:
		ActionBufferCheck();
		MoveBufferCheck();
		break;
	case ECharacterState::E_DOWN:
		if (ActionInputBuffer.Num() > 0) {
			TESTLOG(Warning, TEXT("Down Getup"));
			CanBufferInput = false;
			PlayGetUpAnim.Execute();
			auto Control = Cast<ABasePlayerController>(GetOwner());
			if (Control != nullptr) {
				Control->ChangeStateDel.Broadcast(Control->GetPreState());
			}
			ActionInputBuffer.Reset();
			MoveInputBuffer.Reset();
		}
		break;
	case ECharacterState::E_DEAD:
		break;
	}
}
void  UInputBufferManager::ChangeWeapon(EWeaponType weapon) {
	if (CurrentWeapon == weapon) return;
	CurrentWeapon = weapon;

	switch (CurrentWeapon) {
	case EWeaponType::E_NOWEAPON:
		CommandTable->ChangeCommandTable(EWeaponType::E_NOWEAPON);
		break;
	case EWeaponType::E_DUAL:
		CommandTable->ChangeCommandTable(EWeaponType::E_DUAL);
		break;
	}

	CurrentActionName = ECommandName::E_BASE;
	ChainReset();
}

void UInputBufferManager::SetCommandTable(UComandTableManager* table) {
	CommandTable = table;
}

void UInputBufferManager::InputBufferOpen()
{
	CanBufferInput = true;
}
void UInputBufferManager::InputBufferClose()
{
	CanBufferInput = false;
}
void UInputBufferManager::SetCurrentAction(ECommandName actionName) {
	CurrentActionName = actionName;
}
void UInputBufferManager::SetPressedW(bool value) {
	bPressedW = value;
}
void UInputBufferManager::SetPressedS(bool value) {
	bPressedS = value;
}
void UInputBufferManager::SetPressedD(bool value) {
	bPressedD = value;
}
void UInputBufferManager::SetPressedA(bool value) {
	bPressedA = value;
}

void UInputBufferManager::PlayAnimation(ECommandName actionName) {
	if (StateSpecial&&actionName == ECommandName::E_BASE) {
		auto controller = Cast<ABasePlayerController>(GetOwner());
		controller->SpecialOff();
		ReadyForNextAction(actionName);
	}
	else {
		auto Anim = CommandTable->FindAction(actionName);
		ReadyForNextAction(actionName);
		PlayAnimEventDel.Execute(Anim.ActionMontage);
	}
}
void UInputBufferManager::InputBufferChecker() {
	if (ActionInputBuffer.Num() <= 0) {
		return;
	}
	ChainReset();
}
void UInputBufferManager::SpecialOn() {
	StateSpecial = true;
	ResetActionName = ECommandName::E_SPECIAL;
	ReadyForNextAction(ECommandName::E_SPECIAL);
}
void UInputBufferManager::SpecialOff() {
	StateSpecial = false;
	if (CurrentActionName == ECommandName::E_BASE
		|| CurrentActionName == ECommandName::E_SPECIAL) {
		ResetActionName = ECommandName::E_BASE;
	}
	else {
		ResetActionName = ECommandName::E_DRAW;
	}
}

bool UInputBufferManager::EvadeOrStep() {
	if (CurrentActionName == ECommandName::E_BASE
		|| CurrentActionName == ECommandName::E_DRAW
		|| CurrentActionName == ECommandName::E_SPECIAL
		|| CurrentActionName == ECommandName::E_SPECIALDRAW
		|| CurrentActionName == ECommandName::E_EVADE) {
		if (EvadeAction(ECommandName::E_EVADE)) {
			return true;
		}
	}
	else {
		auto EnableAction = CommandTable->FindEnableAction(CurrentActionName);
		if (EnableAction.Contains(ECommandName::E_LEFTSTEP)) {
			auto Left = MoveInputBuffer.FindByPredicate([](const FMoveInputInfo Lhs) {return Lhs.InputType == EMoveKey::E_LEFT; });
			auto Right = MoveInputBuffer.FindByPredicate([](const FMoveInputInfo Lhs) {return Lhs.InputType == EMoveKey::E_RIGHT; });

			if (Left != nullptr&&Right != nullptr) {
				if (Left->TimeStamp >= Right->TimeStamp) {
					if (EvadeAction(ECommandName::E_LEFTSTEP)) {
						return true;
					}
				}
				else {
					if (EvadeAction(ECommandName::E_RIGHTSTEP)) {
						return true;
					}
				}
			}
			else if (Right != nullptr) {
				if (EvadeAction(ECommandName::E_LEFTSTEP)) {
					return true;
				}
			}
			else if (Left != nullptr) {
				if (EvadeAction(ECommandName::E_RIGHTSTEP)) {
					return true;
				}
			}
		}
	}
	return false;
}
bool UInputBufferManager::EvadeAction(ECommandName actionName) {
	if (EvadeConditionDel.Execute()) {
		PlayAnimation(actionName);
		BufferCheckTime = 0.0f;
		ActionInputBuffer.Reset();
		MoveInputBuffer.Reset();
		return true;
	}
	return false;
}
void UInputBufferManager::ClearInputBuffer() {
	ActionInputBuffer.Reset();
	if (MoveInputBuffer.Num() > 0) {
		MoveInputBuffer.Reset();
	}
}