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
#include "../../UI/FadeWidget.h"
#include "../../UI/GameClearUI.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyNewCharacter.h"

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
	FadePanel->PlayerDeadDel.BindUObject(this, &AMyNewPlayerController::RespawnRequest);
}
void AMyNewPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (IsValid(CameraManager)&& CurrentPlayerState!= ENewPlayerState::E_DEAD) {
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

	if (FadePanel == nullptr) {
		FadePanel = CreateWidget<UFadeWidget>(this, FadePanelClass);
		FadePanel->AddToViewport();
	}
	else {
		FadePanel->BeginFadeIn();
	}

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
}

void AMyNewPlayerController::AttachWidgetToViewport(TSubclassOf<UBaseWidget> widget) {
	if (widget != nullptr) {
		if (PlayerHUD == nullptr) {
			PlayerHUD = CreateWidget<UBaseWidget>(this, widget);
			PlayerHUD->AddToViewport();
			PlayerHUD->Init();
		}
		else {
			PlayerHUD->AddToViewport();
			PlayerHUD->Init();
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
	FadePanel->BeginFadeOut();
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

void AMyNewPlayerController::RespawnRequest() {
	if (IsValid(GetWorld())) {
		Cast<AMyNewGameState>(GetWorld()->GetGameState())->Respawn(*this);
	}
}

void AMyNewPlayerController::HideWidget(bool IsVisible) {
	if (IsVisible == true) {
		PlayerHUD->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		PlayerHUD->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyNewPlayerController::GameClearWidget(TSubclassOf<UGameClearUI>widget) {
	auto ClearWidget = CreateWidget<UGameClearUI>(this, widget);
	ClearWidget->AddToViewport();
	DisableInput(this);
	bShowMouseCursor = true;
}
void AMyNewPlayerController::LookCameraForwardDirection() {
	FVector Target = FVector(GetViewTarget()->GetActorForwardVector().X, GetViewTarget()->GetActorForwardVector().Y, 0);
	FRotator Rotation = FRotator(CurrentCharacter->GetActorRotation().Pitch, GetControlRotation().Yaw, CurrentCharacter->GetActorRotation().Roll);
	CurrentCharacter->SetActorRotation(Rotation);
}