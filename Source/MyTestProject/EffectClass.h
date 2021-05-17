// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectClass.generated.h"

class AActor;
class UCharacterStatusManager;
class ABaseCharacter;

UENUM(BlueprintType)
enum class EEffectType : uint8 {
	E_INVALID = 0 UMETA(DisplayName = "INVALID"),
	E_DUALSPECIAL = 1 UMETA(DisplayName = "DUAL"),
};

UCLASS(Abstract)
class MYTESTPROJECT_API UEffectClass : public UObject
{
	GENERATED_BODY()
public:
	UEffectClass();
protected:
	AActor* Owner;
	EEffectType Identifier;
public:
	virtual void BeginEffect(AActor* owner) PURE_VIRTUAL(UEffectClass::BeginEffect);
	virtual void TickEffect(float delta) PURE_VIRTUAL(UEffectClass::TickEffect);
	virtual void EndEffect() PURE_VIRTUAL(UEffectClass::EndEffect);
	virtual EEffectType GetIdentifier();
};
UCLASS()
class MYTESTPROJECT_API UDualSpecial : public UEffectClass
{
	GENERATED_BODY()
public:
	UDualSpecial();
public:
	virtual void BeginEffect(AActor* owner)  override;
	virtual void TickEffect(float delta) override;
	virtual void EndEffect() override;
private:
	ABaseCharacter* Character;
	FDelegateHandle DelegateHandle;
};