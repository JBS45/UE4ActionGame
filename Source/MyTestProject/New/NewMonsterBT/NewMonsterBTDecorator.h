// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "NewMonsterBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTPROJECT_API UBTDecorator_CheckAttackDistance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckAttackDistance();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
protected:
	UPROPERTY(EditAnywhere, Category = "Min", meta = (AllowPrivateAccess = "true"))
		int32 Min;
	UPROPERTY(EditAnywhere, Category = "Max", meta = (AllowPrivateAccess = "true"))
		int32 Max;
};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_ChcckRandomChance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_ChcckRandomChance();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
protected:
	UPROPERTY(EditAnywhere, Category = "Min", meta = (AllowPrivateAccess = "true"))
		uint8 Min;
	UPROPERTY(EditAnywhere, Category = "Max", meta = (AllowPrivateAccess = "true"))
		uint8 Max;
};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_CheckMonsterBroken : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckMonsterBroken();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:
	UPROPERTY(EditAnywhere, Category = "Broken part", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "ENewMonsterBrokenPart"))
		int32 CheckBrokenType;
};

