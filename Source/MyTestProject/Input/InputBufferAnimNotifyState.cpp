// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferAnimNotifyState.h"
#include "../BaseEnum.h"
#include "../Player/BaseCharacter.h"
#include "../Player/BasePlayerController.h"

static UInputBufferManager* GetInputBuffer(USkeletalMeshComponent * MeshComp) {
	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		auto PlayerController = Cast<ABasePlayerController>(PlayerCharater->Controller);
		if (IsValid(PlayerController)) {
			return PlayerController->GetInputBuffer();
		}
	}
	return nullptr;
}

FString UInputBufferAnimNotifyState::GetNotifyName_Implementation() const {
	return L"InputBuffer";
}
void UInputBufferAnimNotifyState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto buffer = GetInputBuffer(MeshComp);
	if (IsValid(buffer)) {
		if (IsOpen) {
			buffer->InputBufferOpen();
		}
		else {
			buffer->InputBufferClose();
		}
	}
}

void UInputBufferAnimNotifyState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UInputBufferAnimNotifyState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto buffer = GetInputBuffer(MeshComp);
	if (IsValid(buffer)) {
		if (IsOpen) {
			buffer->InputBufferClose();
			buffer->InputBufferChecker();
		}
		else {
			buffer->InputBufferOpen();
		}
	}
}


FString UInputBufferOpen::GetNotifyName_Implementation() const {
	return L"InputBufferOpen";
}
void UInputBufferOpen::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	auto buffer = GetInputBuffer(MeshComp);
	if (IsValid(buffer)) {
		buffer->InputBufferOpen();
	}

}

FString UInputBufferClose::GetNotifyName_Implementation() const {
	return L"InputBufferClose";
}
void UInputBufferClose::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	auto buffer = GetInputBuffer(MeshComp);
	if (IsValid(buffer)) {
		buffer->InputBufferClose();
	}

}


FString UDrawWeapon::GetNotifyName_Implementation() const {
	return L"GoToBattle";
}
void UDrawWeapon::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (IsValid(PlayerCharater)) {
		PlayerCharater->DrawWeapon();
		auto PlayerController = Cast<ABasePlayerController>(PlayerCharater->Controller);
		if (IsValid(PlayerController)) {
			PlayerController->ChangeStateDel.Broadcast(ECharacterState::E_BATTLE);
		}
	}
}


FString UPutUpWeapon::GetNotifyName_Implementation() const {
	return L"GoToIdle";
}
void UPutUpWeapon::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());;
	if (IsValid(PlayerCharater)) {
		PlayerCharater->PutUpWeapon();
		auto PlayerController = Cast<ABasePlayerController>(PlayerCharater->Controller);
		if (IsValid(PlayerController)) {
			PlayerController->ChangeStateDel.Broadcast(ECharacterState::E_IDLE);
		}
	}

}

