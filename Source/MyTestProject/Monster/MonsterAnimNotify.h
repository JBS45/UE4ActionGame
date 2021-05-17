// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MonsterAnimNotify.generated.h"

/**
 *
 */
UCLASS()
class MYTESTPROJECT_API UMonsterRoar : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};


UCLASS()
class MYTESTPROJECT_API UEndPlayAnim : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class MYTESTPROJECT_API UMonsterJumpAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere, Category = "NextSection", meta = (AllowPrivateAccess = "true"))
		FName CurrentSection;
	UPROPERTY(EditAnywhere, Category = "NextSection", meta = (AllowPrivateAccess = "true"))
		FName NextSection;
};

UCLASS()
class MYTESTPROJECT_API UMonsterLaunchToPlayer : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere, Category = "Height", meta = (AllowPrivateAccess = "true"))
		float Height;

};

UCLASS()
class MYTESTPROJECT_API UMonsterRadialDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float Range;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float DamageRate;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float KnockBack;
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		EDamageType DamageType;

};