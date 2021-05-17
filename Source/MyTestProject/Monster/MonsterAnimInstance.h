// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "../MyTestProject.h"
#include"../BaseEnum.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */

UCLASS()
class MYTESTPROJECT_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMonsterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		EMonsterState CurrentState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool IsAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool IsStun;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool IsBattle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float CharSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float YawDir = 0;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Stun;

public:
	void MonsterDead() { IsAlive = false; }
	void SetRightRotate(float value) { YawDir = value;}
	void PlayAnimation(UAnimMontage* montage, float rate);
	void PlayStun();
	void PlayGetUp();
	void ChangeMonsterState(EMonsterState state);
}; 
