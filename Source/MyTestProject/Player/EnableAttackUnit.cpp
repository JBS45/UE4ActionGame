// Fill out your copyright notice in the Description page of Project Settings.



#include "EnableAttackUnit.h"
#include "../BaseWeapon.h"
#include "BaseCharacter.h"
#include "BasePlayerController.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

FString UEnableLeftAttackUnit::GetNotifyName_Implementation() const {
	return L"Left Attack Enable";
}

void UEnableLeftAttackUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->WeaponLeftOverlapOn();
		PlayerCharater->GetLeftHand()->SetDamageRate(DamageRate);
		PlayerCharater->GetLeftHand()->TrailOn();
	}

}

void UEnableLeftAttackUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UEnableLeftAttackUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->WeaponLeftOverlapOff();
		PlayerCharater->GetLeftHand()->TrailOff();

	}
}


FString UEnableRightAttackUnit::GetNotifyName_Implementation() const {
	return L"Right Attack Enable";
}

void UEnableRightAttackUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->WeaponRightOverlapOn();
		PlayerCharater->GetRightHand()->SetDamageRate(DamageRate);
		PlayerCharater->GetRightHand()->TrailOn();
	}

}

void UEnableRightAttackUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UEnableRightAttackUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->WeaponRightOverlapOff();
		PlayerCharater->GetRightHand()->TrailOff();
	}
}


FString UEvadeActionUnit::GetNotifyName_Implementation() const {
	return L"Evade";
}

void UEvadeActionUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->SetEvade(true);
	}

}

void UEvadeActionUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UEvadeActionUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		UGameplayStatics::SetGlobalTimeDilation(PlayerCharater->GetWorld(), 1.0);
		PlayerCharater->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		PlayerCharater->CustomTimeDilation = 1.0f;
		PlayerCharater->SetEvade(false);
	}
}

FString UNoCollisionActionUnit::GetNotifyName_Implementation() const {
	return L"Nocollision";
}

void UNoCollisionActionUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void UNoCollisionActionUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UNoCollisionActionUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	auto PlayerCharater = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharater)) {
		PlayerCharater->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}



FString UChangeCharacterState::GetNotifyName_Implementation() const {
	return L"ChangeState";
}

void UChangeCharacterState::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Player = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(Player)) {
		auto Controller = Cast<ABasePlayerController>(Player->GetController());
		Controller->ChangeStateDel.Broadcast(NextCharacterState);
	}
}
FString UCheckInAir::GetNotifyName_Implementation() const {
	return L"CheckInAir";
}

void UCheckInAir::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Player = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(Player)) {
		if (!Player->GetCharacterMovement()->IsFalling()) {
			Player->GetAnimInst()->Montage_SetNextSection(CurrentSection, NextSection);
		}
	}
}

FString URollBackPreState::GetNotifyName_Implementation() const
{
	return L"PreState";
}
void URollBackPreState::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	auto Player = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(Player)) {
		auto Controller = Cast<ABasePlayerController>(Player->GetController());
		if (IsValid(Controller)) {
			Controller->ChangeStateDel.Broadcast(Controller->GetPreState());
		}
	}
}

FString UCastSpecial::GetNotifyName_Implementation() const
{
	return L"CastingSpecial";
}
void UCastSpecial::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	auto Player = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (IsValid(Player)) {
		auto Controller = Cast<ABasePlayerController>(Player->GetController());
		if (IsValid(Controller)) {
			Controller->SpecialOn();
		}
	}
}