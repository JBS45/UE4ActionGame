// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "../BaseEnum.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MonsterAnimNotifyState.generated.h"

/**
 *
 */
class ABoss;
class ABaseMonster;
class ARockProjectile;

UCLASS()
class MYTESTPROJECT_API UMonsterMeleeAttackUnit : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		EMonsterPartsType AttackEnablePart;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		EDamageType DamageType;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float DamageRate = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float KnockBackDistance = 300.0f;
	TArray<AActor*> OverlappedActor;
	ABaseMonster* Monster;
};

UCLASS()
class MYTESTPROJECT_API UMonsterThrowStone : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		EDamageType DamageType;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float DamageRate = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float KnockBackDistance = 300.0f;
	TArray<AActor*> OverlappedActor;
	ABoss* Monster;
	ARockProjectile* Rock;
};