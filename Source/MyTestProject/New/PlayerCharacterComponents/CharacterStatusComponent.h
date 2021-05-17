// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "CharacterStatusComponent.generated.h"


DECLARE_DELEGATE(FVoidDelegate);

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
	virtual void ReceiveDamage() override;

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
	float OperandField;
	float Critical;
public:
	FVoidDelegate PlayerDeadDel;
	FVoidDelegate StaminaExhaustionDel;

public:
	void ChangeWeaponState(const ENewWeaponType weapon);
	bool GetCalculatedDamage(float& outDamage);
	bool CheckStamina(float amount);
	bool UseStamina(float amount);
	void UseStamina(float amountPerSeconds, float delta);
	void Attach(IUpdateStatus* observer);
private:
	void StaminaRegen(float delta);
	void UpdateStatus();
};
