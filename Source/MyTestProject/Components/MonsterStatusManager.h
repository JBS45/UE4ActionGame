// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "Components/ActorComponent.h"
#include "MonsterStatusManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UMonsterStatusManager : public UActorComponent
{
	GENERATED_BODY()

public:
#define HPPERCENT(percent) (MaxHP/100)*percent
public:
	// Sets default values for this component's properties
	UMonsterStatusManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	FVoidMultiDelegate DeadDel;
	FVoidMultiDelegate BrokenDel;

private:
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float MaxHP;
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float CurrentHP;
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float BaseDamage;

	UPROPERTY(VisibleAnywhere, Category = "BrokenState", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EMonsterBrokenParts"))
		int32 BrokenState;
	int32 StunValue;
public:

	void SetStatus(float maxHp, float baseDamage);
	void SetBrokenState(EMonsterBrokenParts broken);
	void TakeDamage(const int32 acculateDamage);
	void ChangePhase();
	float GetDamage();
};
