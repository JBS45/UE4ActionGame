// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAnimNotifyState.h"
#include "BaseMonster.h"
#include "../Player/BaseCharacter.h"
#include "../Collision/HitCollisionManager.h"
#include "../Components/MyInterface.h"
#include "Boss.h"
#include "RockProjectile.h"
#include "DrawDebugHelpers.h"


FString UMonsterMeleeAttackUnit::GetNotifyName_Implementation() const
{
	return L"Monster Attack Enable";
}

void UMonsterMeleeAttackUnit::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->SetMeleeDamage(DamageRate, KnockBackDistance, DamageType, AttackEnablePart);
	}


}
void UMonsterMeleeAttackUnit::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (IsValid(Monster)) {

		Monster->HitCheck(AttackEnablePart, DamageRate, KnockBackDistance, DamageType);
	}

}
void UMonsterMeleeAttackUnit::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	auto ControllingMonster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (IsValid(ControllingMonster)) {

	}
}

FString UMonsterThrowStone::GetNotifyName_Implementation() const
{
	return L"ThrowStone";
}

void UMonsterThrowStone::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Monster = Cast<ABoss>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Rock = Monster->SpawnProjectile();
	}


}
void UMonsterThrowStone::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (IsValid(Monster)) {
		FVector Location = (MeshComp->GetSocketLocation("Muzzle_01") + MeshComp->GetSocketLocation("Muzzle_02")) / 2;
		Rock->SetActorLocation(Location);
	}

}
void UMonsterThrowStone::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (IsValid(Monster)) {
		Monster->ShootProjectile(Rock);
	}
}