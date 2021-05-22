// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "NewMonsterAnimInstance.generated.h"

class AMyNewBaseMonster;

UCLASS()
class MYTESTPROJECT_API UNewMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UNewMonsterAnimInstance();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner", meta = (AllowPrivateAccess = "true"))
		AMyNewBaseMonster* Owner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewMonsterState CurrentMonsterState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		EMonsterConditionState CurrentConditionState;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float CharSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float TurnSpeed;

public:
	void PlayAnimation(UAnimMontage* montage, float rate);
	void ChangeMonsterState(const ENewMonsterState state);
	void ChangeMonsterConditionState(const EMonsterConditionState state);
public:
	FORCEINLINE void SetTurnSpeed(float value) { TurnSpeed = value; }
};
