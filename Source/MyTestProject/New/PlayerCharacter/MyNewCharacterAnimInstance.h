// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "MyNewCharacterAnimInstance.generated.h"

class AMyNewPlayerController;

UCLASS()
class MYTESTPROJECT_API UMyNewCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMyNewCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
		AMyNewPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewWeaponType WeaponType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewPlayerState PlayerState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewActionState ActionState;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float CharSpeed = 0.0f;
	bool IsPlayingMontageAnim = false;

	IResetInputBuffer* InputBuffer;

	UAnimMontage* LeftHit;
	UAnimMontage* RightHit;
	UAnimMontage* KnockBack;
	UAnimMontage* RoarHit;

private:
	float MakeCharSpeed(APawn* pawn);
	void CheckMontagePlaying();
public:
	void Init();
	void PlayAnimMontage(UAnimMontage* montage);
	void ChangeWeapon(ENewWeaponType type);
	void ChangePlayerState(ENewPlayerState state);
	void ChangeActionState(ENewActionState state);
	void SetInputBuffer(IResetInputBuffer* input);
	void HitEvent(FVector HitDir, FVector right);
	void PlayKnockBack();
	void PlayRoarHit();
};
