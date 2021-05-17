// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecoratorCollection.generated.h"

/**
 *
 */
UCLASS()
class MYTESTPROJECT_API UBTDecorator_CheckDistance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckDistance();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_TargetInRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_TargetInRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
protected:
	UPROPERTY(EditAnywhere, Category = "Min", meta = (AllowPrivateAccess = "true"))
		float Min;
	UPROPERTY(EditAnywhere, Category = "Max", meta = (AllowPrivateAccess = "true"))
		float Max;
};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_RandomChance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_RandomChance();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
protected:
	UPROPERTY(EditAnywhere, Category = "Min", meta = (AllowPrivateAccess = "true"))
		uint8 Min;
	UPROPERTY(EditAnywhere, Category = "Max", meta = (AllowPrivateAccess = "true"))
		uint8 Max;
};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_LeftSide : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_LeftSide();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_RightSide : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_RightSide();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_ForwardSide : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_ForwardSide();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

UCLASS()
class MYTESTPROJECT_API UBTDecorator_CheckBroken : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_CheckBroken();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:
	UPROPERTY(EditAnywhere, Category = "Broken part", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EMonsterBrokenParts"))
		int32 CheckBrokenType;
};

