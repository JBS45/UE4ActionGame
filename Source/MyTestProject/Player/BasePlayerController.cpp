// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "../Monster/BaseMonster.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Components/MyInterface.h"
#include "../Input/InputBufferManager.h"
#include "../Input/CommandTableManager.h"
#include "../EffectClass.h"
#include "../Components/BaseCameraShake.h"



ABasePlayerController::ABasePlayerController() {

	InputBuffer = CreateDefaultSubobject<UInputBufferManager>(TEXT("InputBuffer"));
	CommandTable = CreateDefaultSubobject<UComandTableManager>(TEXT("CommandTable"));

	IsLockOn = false;

	CurrentWeapon = EWeaponType::E_NOWEAPON;
	PreState = ECharacterState::E_IDLE;
}

void ABasePlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("UpDown"), this, &ABasePlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &ABasePlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("LookUP"), this, &ABasePlayerController::CameraLookUp);
	InputComponent->BindAxis(TEXT("Turn"), this, &ABasePlayerController::CameraTurn);

	InputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &ABasePlayerController::ToggleLockOn);
	InputComponent->BindAction(TEXT("LockChnage"), EInputEvent::IE_Pressed, this, &ABasePlayerController::ChangeLockOnTarget);


	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressShift);
	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseShift);

	InputComponent->BindAction(TEXT("Evade"), EInputEvent::IE_Pressed, this, &ABasePlayerController::SpaceKey);
	InputComponent->BindAction(TEXT("MouseLeftClick"), EInputEvent::IE_Pressed, this, &ABasePlayerController::MouseLeftClick);
	InputComponent->BindAction(TEXT("MouseRightClick"), EInputEvent::IE_Pressed, this, &ABasePlayerController::MouseRightClick);

	InputComponent->BindAction(TEXT("W"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressW);
	InputComponent->BindAction(TEXT("W"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseW);
	InputComponent->BindAction(TEXT("S"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressS);
	InputComponent->BindAction(TEXT("S"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseS);
	InputComponent->BindAction(TEXT("D"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressD);
	InputComponent->BindAction(TEXT("D"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseD);
	InputComponent->BindAction(TEXT("A"), EInputEvent::IE_Pressed, this, &ABasePlayerController::PressA);
	InputComponent->BindAction(TEXT("A"), EInputEvent::IE_Released, this, &ABasePlayerController::ReleaseA);

	EKey = FInputActionBinding(TEXT("E"), IE_Pressed);
	MouseThumbKey = FInputActionBinding(TEXT("MouseThumbClick"), IE_Pressed);

	PlayerCameraManager->ViewPitchMin = -60;
	PlayerCameraManager->ViewPitchMax = -10;
}

void ABasePlayerController::BeginPlay() {
	Super::BeginPlay();

	TESTLOG(Warning, TEXT("player control begin play"));

	InputBuffer->SetCommandTable(CommandTable);
}

void ABasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CameraLockOn(DeltaTime);
}

void ABasePlayerController::OnPossess(APawn* pawn) {
	Super::OnPossess(pawn);

}


void ABasePlayerController::MoveForward(float Value)
{
	if (!CanMove) return;

	CurrentCharaceter->MoveForward(Value);
	if (Value > 0.0f) {
		PressW();
	}
	else if (Value < 0.0f) {
		PressS();
	}
}
void ABasePlayerController::MoveRight(float Value)
{
	if (!CanMove) return;

	if (CurrentCharaceter)
	{
		CurrentCharaceter->MoveRight(Value);
		if (Value > 0.0f) {
			PressD();
		}
		else if (Value < 0.0f) {
			PressA();
		}
	}
}

void ABasePlayerController::PressW()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_FORWARD;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
	InputBuffer->SetPressedW(true);
}
void ABasePlayerController::ReleaseW()
{
	InputBuffer->SetPressedW(false);
}
void ABasePlayerController::PressS()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_BACKWARD;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
	InputBuffer->SetPressedS(true);
}
void ABasePlayerController::ReleaseS()
{
	InputBuffer->SetPressedS(false);
}
void ABasePlayerController::PressD()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_RIGHT;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
	InputBuffer->SetPressedD(true);
}
void ABasePlayerController::ReleaseD()
{
	InputBuffer->SetPressedD(false);
}
void ABasePlayerController::PressA()
{
	FMoveInputInfo input;
	input.InputType = EMoveKey::E_LEFT;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
	InputBuffer->SetPressedA(true);
}
void ABasePlayerController::ReleaseA()
{
	InputBuffer->SetPressedA(false);
}
void ABasePlayerController::Special() {
	FActionInputInfo input;
	input.InputType = EActionKey::E_SPECIAL;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
}


void ABasePlayerController::CameraLookUp(float Value) {

	if (CurrentCharaceter)
	{
		CurrentCharaceter->CameraLockUp(Value);
	}
}
void ABasePlayerController::CameraTurn(float Value) {

	if (CurrentCharaceter)
	{
		CurrentCharaceter->CameraTurn(Value);
	}
}
void ABasePlayerController::PressShift() {

	IsSprint = true;
	SprintDel.Broadcast(IsSprint);

	if (CharacterState == ECharacterState::E_BATTLE)
	{
		InputBuffer->PutUpWeapon();
	}
}
void ABasePlayerController::ReleaseShift() {

	IsSprint = false;
	SprintDel.Broadcast(IsSprint);
}

void ABasePlayerController::MouseLeftClick() {
	//AddInputBuffer(EInputKey::E_LEFTCLICK);
	FActionInputInfo input;
	input.InputType = EActionKey::E_LEFTCLICK;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
}
void ABasePlayerController::MouseRightClick() {
	//AddInputBuffer(EInputKey::E_RIGHTCLICK);
	FActionInputInfo input;
	input.InputType = EActionKey::E_RIGHTCLICK;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
}
void ABasePlayerController::SpaceKey() {
	//AddInputBuffer(EInputKey::E_EVADE);
	TESTLOG(Warning, TEXT("What?"));
	FActionInputInfo input;
	input.InputType = EActionKey::E_EVADE;
	input.TimeStamp = GetWorld()->TimeSeconds;
	InputBuffer->AddInputToInputBuffer(input);
}

void ABasePlayerController::ChangeCharacterState(ECharacterState state) {
	if (CharacterState == state) return;
	auto Temp = CharacterState;
	if (Temp == ECharacterState::E_IDLE || Temp == ECharacterState::E_BATTLE) {
		PreState = Temp;
	}
	CharacterState = state;
	switch (CharacterState) {
	case ECharacterState::E_IDLE:
		CanMove = true;
		break;
	case ECharacterState::E_BATTLE:
		CanMove = true;
		break;
	case ECharacterState::E_HIT:
		CanMove = false;
		break;
	case ECharacterState::E_DOWN:
		CanMove = false;
		break;
	case ECharacterState::E_DEAD:
		CanMove = false;
		PlayerHUD->RemoveFromViewport();
		break;
	}
}
void ABasePlayerController::ChangeWeapon(EWeaponType type) {
	if (CurrentWeapon == type)return;
	CurrentWeapon = type;
	switch (CurrentWeapon) {
	case EWeaponType::E_NOWEAPON:
		MouseThumbKey.ActionDelegate.Unbind();
		EKey.ActionDelegate.Unbind();
		break;
	case EWeaponType::E_DUAL:
		MouseThumbKey.ActionDelegate.Unbind();
		EKey.ActionDelegate.BindDelegate(this, &ABasePlayerController::Special);
		break;
	}
	&InputComponent->AddActionBinding(EKey);
	&InputComponent->AddActionBinding(MouseThumbKey);

}
void ABasePlayerController::ToggleLockOn()
{
	if (IsValid(CurrentCharaceter) == false) return;

	if (IsLockOn) {
		CurrentCharaceter->GetLockOnDetect()->LockOff();
	}
	else {
		CurrentCharaceter->GetLockOnDetect()->LockOnCamera();
	}
}

void ABasePlayerController::ChangeLockOnTarget()
{
	/*if (IsLockOn) {
		auto Array = CurrentCharaceter->GetTargetMonster();
		if (Array.Num() > 0) {
			if (Target != nullptr) {
				auto iter = Array.Find(Target);
				iter++;
				if (iter > Array.Num() - 1) {
					iter = 0;
				}
				Target = Cast<ABaseMonster>(Array[iter]);
			}
		}
	}
	//Todo : Next Target from iterator;*/
}
void ABasePlayerController::AttachWidgetToViewport(TSubclassOf<class UBaseWidget> widget) {
	if (widget != nullptr) {
		PlayerHUD = CreateWidget<UBaseWidget>(this, widget);

		if (PlayerHUD != nullptr) {
			PlayerHUD->AddToViewport();
		}
	}
}
void ABasePlayerController::CameraLockOn(float deltaTime) {
	if (IsLockOn) {
		Target = CurrentCharaceter->GetLockOnDetect()->GetLockTarget();
		auto DestRotation = UKismetMathLibrary::FindLookAtRotation(CurrentCharaceter->GetActorLocation(), Target->GetActorLocation());
		auto TempRot = FMath::RInterpTo(GetControlRotation(), DestRotation, deltaTime, 10.0f);
		if (TempRot.Pitch < PlayerCameraManager->ViewPitchMax) {
			SetControlRotation(TempRot);
		}
		else {
			TempRot.Pitch = PlayerCameraManager->ViewPitchMax;
			SetControlRotation(TempRot);
		}

		PlayerHUD->TraceTarget(this, Target);

		float Turn = FMath::Abs(InputComponent->GetAxisValue("Turn"));
		float LookUp = FMath::Abs(InputComponent->GetAxisValue("LookUP"));
		float Distance = FVector::Distance(CurrentCharaceter->GetActorLocation(), Target->GetActorLocation());
		if (Turn >= 2.0f || LookUp >= 2.0f || Distance > CurrentCharaceter->GetDetectRange()) {
			CurrentCharaceter->GetLockOnDetect()->LockOff();
		}
	}
}


ABaseMonster* ABasePlayerController::GetTarget() {
	return Target;
}

void ABasePlayerController::OnDead() {
	ChangeStateDel.Broadcast(ECharacterState::E_DEAD);
}
void ABasePlayerController::NoStatmina() {
	IsSprint = false;
	SprintDel.Broadcast(false);
}
UInputBufferManager* ABasePlayerController::GetInputBuffer() {
	return InputBuffer;
}
ECharacterState ABasePlayerController::GetPreState() const {
	return PreState;
}

void ABasePlayerController::SpecialOn() {
	InputBuffer->SpecialOn();
	auto Effect = NewObject<UDualSpecial>();
	Effect->BeginEffect(CurrentCharaceter);
	CurrentCharaceter->SetEffects(Effect);
}
void ABasePlayerController::SpecialOff() {
	auto Effects = CurrentCharaceter->GetEffects();
	auto element = Effects->FindByPredicate([](UEffectClass* Lhs) {return (Lhs->GetIdentifier() == EEffectType::E_DUALSPECIAL); });
	(*element)->EndEffect();
}
void ABasePlayerController::PossessInit(APawn* newPawn) {

	CurrentCharaceter = Cast<ABaseCharacter>(newPawn);
	CurrentCharaceter->SetInitWeapon();

	if (CurrentCharaceter == nullptr)  return;

	/*Change State*/
	ChangeStateDel.AddUObject(this, &ABasePlayerController::ChangeCharacterState);
	ChangeStateDel.AddUObject(CurrentCharaceter, &ABaseCharacter::CharacterChangeState);
	ChangeStateDel.AddUObject(InputBuffer, &UInputBufferManager::ChangeCharacterState);
	ChangeStateDel.AddUObject(CurrentCharaceter->GetAnimInst(), &UBaseCharAnimInstance::CharacterChangeState);

	/*Change Weapon*/
	ChangeWeaponDel.AddUObject(this, &ABasePlayerController::ChangeWeapon);
	ChangeWeaponDel.AddUObject(CurrentCharaceter, &ABaseCharacter::ChangeWeapon);
	ChangeWeaponDel.AddUObject(CommandTable, &UComandTableManager::ChangeCommandTable);
	ChangeWeaponDel.AddUObject(InputBuffer, &UInputBufferManager::ChangeWeapon);
	ChangeWeaponDel.AddUObject(CurrentCharaceter->GetAnimInst(), &UBaseCharAnimInstance::ChangeWeapon);


	InputBuffer->PlayAnimEventDel.BindUObject(CurrentCharaceter->GetAnimInst(), &UBaseCharAnimInstance::PlayAnimMontage);
	InputBuffer->PlayGetUpAnim.BindUObject(CurrentCharaceter->GetAnimInst(), &UBaseCharAnimInstance::PlayGetUp);
	InputBuffer->EvadeConditionDel.BindUObject(CurrentCharaceter->GetCharacterStatus(), &UCharacterStatusManager::EvadeStamina);

	SprintDel.AddUObject(CurrentCharaceter, &ABaseCharacter::SetIsSprint);


	CurrentCharaceter->GetAnimInst()->ChainResetDel.BindUObject(InputBuffer, &UInputBufferManager::ChainReset);

	CurrentCharaceter->GetCharacterStatus()->OnCharacterDeadDel.BindUObject(this, &ABasePlayerController::OnDead);
	CurrentCharaceter->GetCharacterStatus()->OnStaminaZeroDel.AddUObject(this, &ABasePlayerController::NoStatmina);


	AttachWidgetToViewport(HUDWidgetClass);
	//CommandTable->Attach(PlayerHUD);

	CurrentCharaceter->GetLockOnDetect()->Attach(this);
	CurrentCharaceter->AttackDel.BindUObject(PlayerHUD, &UBaseWidget::UseDamageText);

}
void ABasePlayerController::PlayCharacter()
{
	ChangeStateDel.Broadcast(ECharacterState::E_IDLE);
	ChangeWeaponDel.Broadcast(EWeaponType::E_DUAL);
}

void ABasePlayerController::NotifyLockOnData(bool isLockOn, ABaseMonster* target) {

	if (this->IsLockOn == false && isLockOn == true) {
		PlayerHUD->SetCameraLockOn(true);
	}
	if (this->IsLockOn == true && isLockOn == false) {
		PlayerHUD->SetCameraLockOn(false);
	}
	this->IsLockOn = isLockOn;
	this->Target = target;

}

void ABasePlayerController::CameraShake() {
	PlayerCameraManager->PlayCameraShake(MyShake, 1.0f);
}
