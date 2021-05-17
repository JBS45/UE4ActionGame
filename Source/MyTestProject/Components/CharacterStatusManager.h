// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseStatus.h"
#include "../BaseEnum.h"
#include "Components/ActorComponent.h"
#include "MyInterface.h"
#include "CharacterStatusManager.generated.h"




UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UCharacterStatusManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UCharacterStatusManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	const float ReGenBaseTime = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "ECharacterStaminaUse"))
		int32 StaminaUseFlag;

	bool IsAlive;

	float BuffRate = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		FBaseCharacterStatus CharacterStatus;
	TArray<IStatusInterface*> StatusObservers;
public:
	FVoidDelegate OnCharacterDeadDel;
	FVoidMultiDelegate OnStaminaZeroDel;

	// Called every frame

	void StatusInit();
	void EquipItem(const FEquipmentStatus item);

	float GetDamage();
	float GetCritical();

	void ConsumeStamina(float value, float delta);
	bool EvadeStamina();
	int32 TakeDamage(float Damage);

	FORCEINLINE void Attach(IStatusInterface* newObserver);
	FORCEINLINE float GetConsumeSprint() const;
	FORCEINLINE void StaminaUse(ECharacterStaminaUse type, bool IsOn);
	FORCEINLINE void SetBuffRate(float value);
private:
	void CreateCharState();
	void UpdateStatus();

	void RegenStamina(float delta);
	void Notify();
};
