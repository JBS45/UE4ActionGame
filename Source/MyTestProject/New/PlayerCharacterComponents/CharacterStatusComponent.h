// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "CharacterStatusComponent.generated.h"


DECLARE_DELEGATE(FVoidDelegate);
DECLARE_MULTICAST_DELEGATE(FVoidMultiDelegate);

class AMyNewCharacter;

UCLASS()
class MYTESTPROJECT_API UCharacterStatusComponent : public UStatusComponent
{
	GENERATED_BODY()
public:
	UCharacterStatusComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitStatus(APawn* owner) override;
private:
	AMyNewCharacter* Owner;

	TArray<IUpdateStatus*> Observers;

	float MaxHp;
	float CurrentHp;

	float MaxStamina;
	float CurrentStamina;

	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "ENewStaminaState"))
		int32 StaminaFlag;

	float WeaponDamage;
	float WeaponCondDamage;
	float OperandField;
	float Critical;
public:
	FVoidDelegate PlayerDeadDel;
	FVoidMultiDelegate StaminaExhaustionDel;

public:
	void ChangeWeaponState(const ENewWeaponType weapon);
	bool GetCalculatedDamage(float& outDamage);
	bool CheckStamina(float amount);
	bool UseStamina(float amount);
	void UseStamina(float amountPerSeconds, float delta);
	void Attach(IUpdateStatus* observer);
	void TakeDamage(float Damage);
	void SetWeaponData(float damage, float conddamage, float critical);
	void RegenHp(float amountPerSeconds, float delta);

	FORCEINLINE float GetDamage() { return WeaponDamage * OperandField; }
	FORCEINLINE float GetCondDamage() { return WeaponCondDamage; }
	FORCEINLINE float GetCritical() { return Critical * OperandField; }
	FORCEINLINE void SetOperandField(float value) { OperandField = value; }
	FORCEINLINE void AddStaminaFlag(ENewStaminaState flag) { StaminaFlag |= (int32)flag; }
	FORCEINLINE void SubStaminaFlag(ENewStaminaState flag) { StaminaFlag &= ((int32)~flag); }
private:
	void StaminaRegen(float delta);
	void UpdateStatus();
};
