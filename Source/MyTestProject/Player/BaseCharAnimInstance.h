// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "Animation/AnimInstance.h"
#include "BaseCharAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class MYTESTPROJECT_API UBaseCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UBaseCharAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ECharacterState CurrentState = ECharacterState::E_NONE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsBattle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float CharSpeed;


	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CommonAnim", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* KnockBack;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CommonAnim", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitRoar;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "CommonAnim", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* GetUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		EWeaponType CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		FRotator LeftFootRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		FRotator RightFootRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float LeftFootIKValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float RightFootIKValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float HipOffset;

private:
	bool IsPlayingMontageAnim = false;
private:
	float MakeCharSpeed(APawn* pawn);
	void CheckMontagePlaying();
public:
	FVoidDelegate ChainResetDel;

	void ExchangeWeapon(EWeaponType type);
	void PlayAnimMontage(UAnimMontage* montage);
	void PlayKnockBack();
	void PlayHitRoar();
	void PlayGetUp();

	void CharacterChangeState(ECharacterState state);
	void ChangeWeapon(EWeaponType type);
};
