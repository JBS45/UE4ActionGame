// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MyNewPlayerInterface.h"
#include "MyNewInputInterface.h"
#include "AnimNotifyCollection.generated.h"


class AMyNewCharacter;
class AMyNewPlayerController;
class UMyNewInputBuffer;
class USoundCue;


class NewNotifyUtills {
public:
	static AMyNewCharacter* GetCharacter(USkeletalMeshComponent* MeshComp);
	static AMyNewPlayerController* GetController(USkeletalMeshComponent* MeshComp);
	static UMyNewInputBuffer* GetInputBuffer(USkeletalMeshComponent* MeshComp);
};

UCLASS()
class MYTESTPROJECT_API UPlayerStateChange : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	UPROPERTY(EditAnywhere, Category = "PlayerState", meta = (AllowPrivateAccess = "true"))
		ENewPlayerState ChangeState;
};

UCLASS()
class MYTESTPROJECT_API UPlayerCheckSprint : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class MYTESTPROJECT_API UPlayerDraw : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class MYTESTPROJECT_API UPlayerPutup : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class MYTESTPROJECT_API UChangeWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};


UCLASS()
class MYTESTPROJECT_API UChangeInputBufferState : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	UPROPERTY(EditAnywhere, Category = "BufferState", meta = (AllowPrivateAccess = "true"))
		EInputBufferState ChangeState;
};

UCLASS()
class MYTESTPROJECT_API UActionState : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere, Category = "ActionState", meta = (AllowPrivateAccess = "true"))
		ENewActionState ChangeState;
};

UCLASS()
class MYTESTPROJECT_API UPlaySound : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		class USoundCue* SoundCue;
};

UCLASS()
class MYTESTPROJECT_API UAfterAction : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

};

UCLASS()
class MYTESTPROJECT_API UCastBuff : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

};

UCLASS()
class MYTESTPROJECT_API UKnockbackCheckFalling : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	UPROPERTY(EditAnywhere, Category = "NextSection", meta = (AllowPrivateAccess = "true"))
		FName CurrentSection;
	UPROPERTY(EditAnywhere, Category = "NextSection", meta = (AllowPrivateAccess = "true"))
		FName NextSection;
};



UCLASS()
class MYTESTPROJECT_API UInputBufferStateControl : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
	UPROPERTY(EditAnywhere, Category = "BufferState", meta = (AllowPrivateAccess = "true"))
		EInputBufferState StartState;
	UPROPERTY(EditAnywhere, Category = "BufferState", meta = (AllowPrivateAccess = "true"))
		EInputBufferState EndState;
};

UCLASS()
class MYTESTPROJECT_API UInputBufferGetNextInput : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class MYTESTPROJECT_API UNewCharacterEvade : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};

UCLASS()
class MYTESTPROJECT_API UDrinkPotion : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
private:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
private:
	class UCharacterStatusComponent* status;
};
