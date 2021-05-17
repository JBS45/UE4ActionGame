// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


UCLASS(Abstract,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTESTPROJECT_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void InitStatus(APawn* owner) PURE_VIRTUAL(UStatusComponent::InitStatus);
	virtual void ReceiveDamage() PURE_VIRTUAL(UStatusComponent::ReceiveDamage);
};
