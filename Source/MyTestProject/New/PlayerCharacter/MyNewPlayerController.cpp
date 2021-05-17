// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewPlayerController.h"
#include "MyNewCharacter.h"
#include "../PlayerCharacterComponents/MyNewInputBuffer.h"
#include "../PlayerCharacterComponents/MyNewCommandTable.h"
#include "../../UI/BaseWidget.h"


AMyNewPlayerController::AMyNewPlayerController() {
	InputBuffer = CreateDefaultSubobject<UMyNewInputBuffer>(TEXT("InputBuffer"));
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

	InputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::LockOn);
	InputComponent->BindAction(TEXT("WeaponChange"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::WeaponChange);
	InputComponent->BindAction(TEXT("Cast"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::Cast);
	InputComponent->BindAction(TEXT("Potion"), EInputEvent::IE_Pressed, InputBuffer, &UMyNewInputBuffer::Potion);

}
void AMyNewPlayerController::OnPossess(APawn* pawn) {
	Super::OnPossess(pawn);

	PossessProcess(pawn);
}

void AMyNewPlayerController::PossessProcess(APawn* pawn) {
	CurrentCharacter = Cast<AMyNewCharacter>(pawn);
	if (CurrentCharacter != nullptr) {
		InputBuffer->SetCurrentCharacter(CurrentCharacter);
		InputBuffer->SetPlayerController(this);
	}

	CurrentCharacter->SetPlayerController(this);

	ChangePlayerState.AddUObject(CurrentCharacter, &AMyNewCharacter::ChangePlayerState);
	ChangePlayerState.AddUObject(InputBuffer, &UMyNewInputBuffer::ChangePlayerState);

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
}

UMyNewInputBuffer* AMyNewPlayerController::GetInputBuffer() {
	return InputBuffer;
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

void AMyNewPlayerController::PlayerDead() {
	ChangePlayerState.Broadcast(ENewPlayerState::E_DEAD);
}
void AMyNewPlayerController::StaminaExhuastion() {
	if (CurrentPlayerState == ENewPlayerState::E_SPRINT) {
		ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);
	}
}
UBaseWidget* AMyNewPlayerController::GetPlayerHUD() {
	return PlayerHUD;
}