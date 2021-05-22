// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionNotify.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../NewWeapon/MyNewWeaponManager.h"
#include "../NewWeapon/MyNewBaseWeapon.h"
#include "../PlayerCharacter/MyNewPlayerController.h"

UMyNewWeaponManager* WeaponNotifyUtills::GetWeaponManager(USkeletalMeshComponent* MeshComp) {
	auto Player =Cast<AMyNewCharacter>(MeshComp->GetOwner());
	if (IsValid(Player)) {
		return Player->GetWeaponManager();
	}
	return nullptr;
}

FString ULeftWeapon::GetNotifyName_Implementation() const {
	return L"LeftWeaponAttack";
}

void ULeftWeapon::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	Manager = WeaponNotifyUtills::GetWeaponManager(MeshComp);
	if (IsValid(Manager)) {
		Weapon = Manager->GetAttackWeapon(EWeaponHand::E_LEFT);
		if (SFX == nullptr) {
			Weapon->PlaySwingAudio();
		}
		else {
			Weapon->PlaySwingAudio(SFX);
		}
	}
}
void ULeftWeapon::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) {
	if (IsValid(Manager)) {
		Weapon->HitResult(DamageRate);
	}
}
void ULeftWeapon::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	if (IsValid(Manager)) {
		Weapon->ResetDamagedMonster();
		auto Player = Cast<AMyNewCharacter>(MeshComp->GetOwner());
		if (IsValid(Player)) {
			Player->GetPlayerController()->ChangeActionState.Broadcast(ENewActionState::E_AFTERACTION);
		}
	}
}

FString URightWeapon::GetNotifyName_Implementation() const {
	return L"RightWeaponAttack";
}

void URightWeapon::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	Manager = WeaponNotifyUtills::GetWeaponManager(MeshComp);
	if (IsValid(Manager)) {
		Weapon = Manager->GetAttackWeapon(EWeaponHand::E_RIGHT);
		if (SFX == nullptr) {
			Weapon->PlaySwingAudio();
		}
		else {
			Weapon->PlaySwingAudio(SFX);
		}
	}
}
void URightWeapon::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) {
	if (IsValid(Manager)) {
		Weapon->HitResult(DamageRate);
	}
}
void URightWeapon::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	if (IsValid(Manager)) {
		Weapon->ResetDamagedMonster();
		auto Player = Cast<AMyNewCharacter>(MeshComp->GetOwner());
		if (IsValid(Player)) {
			Player->GetPlayerController()->ChangeActionState.Broadcast(ENewActionState::E_AFTERACTION);
		}
	}
}

FString ULeftTrail::GetNotifyName_Implementation() const {
	return L"LeftWeaponTrail";
}

void ULeftTrail::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	Manager = WeaponNotifyUtills::GetWeaponManager(MeshComp);
	if (IsValid(Manager)) {
		Weapon = Manager->GetAttackWeapon(EWeaponHand::E_LEFT);
		Weapon->BeginTrail();
	}
}
void ULeftTrail::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	if (IsValid(Manager)) {
		Weapon->EndTrail();
	}
}


FString URightTrail::GetNotifyName_Implementation() const {
	return L"RightWeaponTrail";
}

void URightTrail::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {
	Manager = WeaponNotifyUtills::GetWeaponManager(MeshComp);
	if (IsValid(Manager)) {
		Weapon = Manager->GetAttackWeapon(EWeaponHand::E_RIGHT);
		Weapon->BeginTrail();

	}
}
void URightTrail::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	if (IsValid(Manager)) {
		Weapon->EndTrail();
	}
}

