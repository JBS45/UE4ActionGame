// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "InputBufferManager.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "InputBufferAnimNotifyState.generated.h"


/**
 *
 */

class MYTESTPROJECT_API NotifyUtills {
public:
	static UInputBufferManager* GetInputBuffer(USkeletalMeshComponent* MeshComp);
};

UCLASS()
class MYTESTPROJECT_API UInputBufferAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	UPROPERTY(EditAnywhere, Category = "InputBufferState", meta = (AllowPrivateAccess = "true"))
		bool IsOpen = true;
};
UCLASS()
class MYTESTPROJECT_API UInputBufferOpen : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

};
UCLASS()
class MYTESTPROJECT_API UInputBufferClose : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

};

UCLASS()
class MYTESTPROJECT_API UDrawWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

};
UCLASS()
class MYTESTPROJECT_API UPutUpWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

};
