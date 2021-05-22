// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyCollection.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../PlayerCharacter/MyNewPlayerController.h"
#include "../PlayerCharacterComponents/MyNewInputBuffer.h"
#include "../NewWeapon/MyNewWeaponManager.h"
#include "../PlayerCharacterComponents/CharacterStatusComponent.h"

AMyNewCharacter* NewNotifyUtills::GetCharacter(USkeletalMeshComponent* MeshComp) {
	auto PlayerCharater = Cast<AMyNewCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		return PlayerCharater;
	}
	return nullptr;
}
AMyNewPlayerController* NewNotifyUtills::GetController(USkeletalMeshComponent* MeshComp) {
	if (GetCharacter(MeshComp) != nullptr) {
		return GetCharacter(MeshComp)->GetPlayerController();
	}
	return nullptr;
}
UMyNewInputBuffer* NewNotifyUtills::GetInputBuffer(USkeletalMeshComponent* MeshComp) {
	if (GetController(MeshComp) != nullptr) {
		return GetController(MeshComp)->GetInputBuffer();
	}
	return nullptr;
}

FString UPlayerStateChange::GetNotifyName_Implementation() const {
	return L"ChangePlayerState";
}

void UPlayerStateChange::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto PlayerController = NewNotifyUtills::GetController(MeshComp);
	if (IsValid(PlayerController)) {
			PlayerController->ChangePlayerState.Broadcast(ChangeState);
	}
}


FString UPlayerCheckSprint::GetNotifyName_Implementation() const {
	return L"CheckSprint";
}

void UPlayerCheckSprint::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto PlayerController = NewNotifyUtills::GetController(MeshComp);
	if (IsValid(PlayerController)) {
		if (PlayerController->GetInputBuffer()->GetIsSprintFlag()) {
			PlayerController->ChangePlayerState.Broadcast(ENewPlayerState::E_SPRINT);
		}
		else {
			PlayerController->ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);
		}
	}
	
}
FString UPlayerDraw::GetNotifyName_Implementation() const {
	return L"EquipWeapon";
}

void UPlayerDraw::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto character = NewNotifyUtills::GetCharacter(MeshComp);
	if (IsValid(character)) {
		character->GetWeaponManager()->Draw();
		auto PlayerController = NewNotifyUtills::GetController(MeshComp);
		if (IsValid(PlayerController)) {
			PlayerController->ChangePlayerState.Broadcast(ENewPlayerState::E_BATTLE);
		}
	}
}
FString UPlayerPutup::GetNotifyName_Implementation() const {
	return L"UnequipWeapon";
}

void UPlayerPutup::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto character = NewNotifyUtills::GetCharacter(MeshComp);
	if (IsValid(character)) {
		character->GetWeaponManager()->PutUp();
		auto PlayerController = NewNotifyUtills::GetController(MeshComp);
		if (IsValid(PlayerController)) {
			PlayerController->ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);
		}
	}
}

FString UChangeWeapon::GetNotifyName_Implementation() const {
	return L"ChangeNextWeapon";
}

void UChangeWeapon::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto character = NewNotifyUtills::GetCharacter(MeshComp);
	if (IsValid(character)) {
		auto NextWeapon = character->GetWeaponManager()->GetNextWeapon();
		auto PlayerController = NewNotifyUtills::GetController(MeshComp);
		if (IsValid(PlayerController)) {
			PlayerController->ChangeWeaponType.Broadcast(NextWeapon);
		}
	}
}


FString UChangeInputBufferState::GetNotifyName_Implementation() const {
	return L"ChangeInputBufferState";
}

void UChangeInputBufferState::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto InputBuffer = NewNotifyUtills::GetInputBuffer(MeshComp);
	if (InputBuffer != nullptr) {
		InputBuffer->InputBufferStateChange(ChangeState);
	}
}

FString UActionState::GetNotifyName_Implementation() const {
	return L"ChangeActionState";
}

void UActionState::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {

	auto PlayerController = NewNotifyUtills::GetController(MeshComp);
	if (IsValid(PlayerController)) {
		PlayerController->ChangeActionState.Broadcast(ChangeState);
	}
}


FString UPlaySound::GetNotifyName_Implementation() const {
	return L"PlayerSound";
}

void UPlaySound::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {

	auto Player= NewNotifyUtills::GetCharacter(MeshComp);
	if (IsValid(Player)) {
		Player->PlaySound(SoundCue);
	}
}

FString UAfterAction::GetNotifyName_Implementation() const {
	return L"AfterAction";
}

void UAfterAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {

	auto PlayerController = NewNotifyUtills::GetController(MeshComp);
	if (IsValid(PlayerController)) {
		if (PlayerController->GetInputBuffer()) {
			PlayerController->ChangeActionState.Broadcast(ENewActionState::E_AFTERACTION);
		}
	}
}

FString UCastBuff::GetNotifyName_Implementation() const {
	return L"CastBuff";
}

void UCastBuff::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {

	auto character = NewNotifyUtills::GetCharacter(MeshComp);
	if (IsValid(character)) {
		character->DualBuff();
	}
}


FString UInputBufferStateControl::GetNotifyName_Implementation() const {
	return L"InputBufferStateControl";
}
void UInputBufferStateControl::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	auto InputBuffer = NewNotifyUtills::GetInputBuffer(MeshComp);
	if (InputBuffer != nullptr) {
		InputBuffer->InputBufferStateChange(StartState);
	}

}
void UInputBufferStateControl::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto InputBuffer = NewNotifyUtills::GetInputBuffer(MeshComp);
	if (InputBuffer != nullptr) {
		InputBuffer->InputBufferStateChange(EndState);
	}
}

FString UInputBufferGetNextInput::GetNotifyName_Implementation() const {
	return L"GetNextIpnut";
}
void UInputBufferGetNextInput::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	auto InputBuffer = NewNotifyUtills::GetInputBuffer(MeshComp);
	if (InputBuffer != nullptr) {
		InputBuffer->InputBufferStateChange(EInputBufferState::E_ONLYACTION);
	}

}
void UInputBufferGetNextInput::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto InputBuffer = NewNotifyUtills::GetInputBuffer(MeshComp);
	if (InputBuffer != nullptr) {
		InputBuffer->InputBufferStateChange(EInputBufferState::E_ONLYMOVE);
	}
}


FString UNewCharacterEvade::GetNotifyName_Implementation() const {
	return L"CharacterEvade";
}

void UNewCharacterEvade::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	auto PlayerController = NewNotifyUtills::GetController(MeshComp);
	if (IsValid(PlayerController)) {
		if (PlayerController->GetInputBuffer()) {
			PlayerController->ChangeActionState.Broadcast(ENewActionState::E_EVADE);
		}
	}

}
void UNewCharacterEvade::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto PlayerController = NewNotifyUtills::GetController(MeshComp);
	if (IsValid(PlayerController)) {
		if (PlayerController->GetInputBuffer()) {
			PlayerController->ChangeActionState.Broadcast(ENewActionState::E_AFTERACTION);
			NewNotifyUtills::GetCharacter(MeshComp)->ResetEvadeFlag();
		}
	}
}


FString UDrinkPotion::GetNotifyName_Implementation() const {
	return L"DrinkPotion";
}

void UDrinkPotion::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	auto PlayerController = NewNotifyUtills::GetController(MeshComp);
	if (IsValid(PlayerController)) {
		PlayerController->GetPotionCoolTime()->Use();
		status = NewNotifyUtills::GetCharacter(MeshComp)->GetStatusManager();
	}
}
void UDrinkPotion::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) {
	if (status!=nullptr) {
		status->RegenHp(10.0f, FrameDeltaTime);
	}

}
void UDrinkPotion::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {

}