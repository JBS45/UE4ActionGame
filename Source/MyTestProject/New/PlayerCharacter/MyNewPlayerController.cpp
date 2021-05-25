// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewPlayerController.h"
#include "MyNewCharacter.h"
#include "../PlayerCharacterComponents/MyNewInputBuffer.h"
#include "../PlayerCharacterComponents/MyNewCommandTable.h"
#include "../../UI/BaseWidget.h"
#include "Templates/UniquePtr.h"
#include "../MyNewGameState.h"
#include "../PlayerCharacterComponents/NewPlayerCameraManager.h"
#include "../PlayerCharacterComponents/GhostTrail.h"
#include "../PlayerCharacterComponents/GhostTrailManager.h"


AMyNewPlayerController::AMyNewPlayerController() {
	InputBuffer = CreateDefaultSubobject<UMyNewInputBuffer>(TEXT("InputBuffer"));
	
	PlayerCameraManagerClass = ANewPlayerCameraManager::StaticClass();
}

void AMyNewPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("UpDown"), InputBuffer, &UMyNewInputBuffer::MoveForward);
	InputComponent->BindAxis(TEXT("LeftRight"), InputBuffer, &UMyNewInputBuffer::MoveRight);
	InputComponent->BindAxis(TEXT("LookUP"), InputBuffer, &UMyNewInputBuffer::CameraLookUp);
	InputComponent->BindAxis(TEXT("Turn"), InputBuffer, &UMyNewInputBuffer::CameraTurn);

	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::PressShift);
	InputComponent->BindAction(TEXT("LeftShift"), EInputEvent::IE_Released, InputBuffer, &UMyNewInputBuffer::ReleaseShift);

	InputComponent->BindAction(TEXT("Evade"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::Evade);
	InputComponent->BindAction(TEXT("MouseLeftClick"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::LeftClick);
	InputComponent->BindAction(TEXT("MouseRightClick"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::RightClick);

	InputComponent->BindAction(TEXT("WeaponChange"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::WeaponChange);
	InputComponent->BindAction(TEXT("Cast"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::Cast);
	InputComponent->BindAction(TEXT("Potion"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::Potion);

	InputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &AMyNewPlayerController::LockOn);
	InputComponent->BindAction(TEXT("LockOnChange"), EInputEvent::IE_Pressed, this, &AMyNewPlayerController::ChangeLockOnTarget);
}
void AMyNewPlayerController::OnPossess(APawn* pawn) {
	PossessProcess(pawn);
	Super::OnPossess(pawn);
	CameraManager = Cast<ANewPlayerCameraManager>(PlayerCameraManager);
}

void AMyNewPlayerController::BeginPlay() {
	Super::BeginPlay();
}
void AMyNewPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (IsValid(CameraManager)) {
		auto Target = CameraManager->CameraLockOn(DeltaTime);
		PlayerHUD->TraceTarget(this,Target);
	}
	PotionCoolTime->Timer(DeltaTime);
	PlayerHUD->UpdatePotion(PotionCoolTime->GetRemainTime(), PotionCoolTime->GetPotionCoolTime());
}
void AMyNewPlayerController::PossessProcess(APawn* pawn) {
	CurrentCharacter = Cast<AMyNewCharacter>(pawn);
	if (CurrentCharacter != nullptr) {
		InputBuffer->SetCurrentCharacter(CurrentCharacter);
		InputBuffer->SetPlayerController(this);
	}

	CurrentCharacter->SetPlayerController(this);

	if (PotionCoolTime == nullptr) {
		PotionCoolTime = TUniquePtr<PotionTimer>(new PotionTimer());
	}

	ChangePlayerState.AddUObject(CurrentCharacter, &AMyNewCharacter::ChangePlayerState);
	ChangePlayerState.AddUObject(InputBuffer, &UMyNewInputBuffer::ChangePlayerState);

	ChangeActionState.AddUObject(this, &AMyNewPlayerController::ChangeAction);
	ChangeActionState.AddUObject(CurrentCharacter, &AMyNewCharacter::ChangeActionState);
	ChangeActionState.AddUObject(InputBuffer, &UMyNewInputBuffer::ChangeActionState);

	ChangeWeaponType.AddUObject(CurrentCharacter, &AMyNewCharacter::ChangeWeaponState);
	ChangeWeaponType.AddUObject(InputBuffer, &UMyNewInputBuffer::ChangeWeaponState);

	AttachWidgetToViewport(HUDWidgetClass);

	ChangeWeaponType.AddUObject(PlayerHUD, &UBaseWidget::ChangeWeapon);
	InputBuffer->GetCommandTable()->Attach(PlayerHUD);
}
void AMyNewPlayerController::UnPoessessProcess() {
	InputBuffer->ClearCurrentCharacter();
	ChangePlayerState.Clear();
	ChangeActionState.Clear();
	ChangeWeaponType.Clear();

	PlayerHUD->RemoveFromViewport();

	if (IsValid(GetWorld())) {
		//Cast<AMyNewGameState>(GetWorld()->GetGameState())->Respawn(*this);
	}
}

void AMyNewPlayerController::AttachWidgetToViewport(TSubclassOf<UBaseWidget> widget) {
	if (widget != nullptr) {
		if (PlayerHUD == nullptr) {
			PlayerHUD = CreateWidget<UBaseWidget>(this, widget);
			PlayerHUD->AddToViewport();
		}
		else{
			PlayerHUD->AddToViewport();
		}
	}
}

void  AMyNewPlayerController::ChangeAction(const ENewActionState state) {
	if (CurrentActionState == state) return;
	CurrentActionState = state;
}

void AMyNewPlayerController::PlayerDead() {
	ChangePlayerState.Broadcast(ENewPlayerState::E_DEAD);
	Cast<AMyNewGameState>(GetWorld()->GetGameState())->PlayerDead(*this);
};

ANewPlayerCameraManager* AMyNewPlayerController::GetCameraManager() {
	return Cast<ANewPlayerCameraManager>(PlayerCameraManager);
}

void AMyNewPlayerController::LockOn() {
	if (IsValid(CameraManager)) {
		CameraManager->ToggleLockOn();
	}
}
void AMyNewPlayerController::ChangeLockOnTarget() {
	if (IsValid(CameraManager)) {
		CameraManager->ChangeLockOn();
	}
}