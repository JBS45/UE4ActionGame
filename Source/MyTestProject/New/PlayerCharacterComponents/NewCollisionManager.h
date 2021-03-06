// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewCollisionManager.generated.h"

class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTESTPROJECT_API UNewCollisionManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNewCollisionManager();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Block", meta = (AllowPrivateAccess = "true"))
		float BlockRadius;
	UPROPERTY(EditAnywhere, Category = "Block", meta = (AllowPrivateAccess = "true"))
		float BlockHeight;
	UPROPERTY(EditAnywhere, Category = "Block", meta = (AllowPrivateAccess = "true"))
		float LerpSpeed;
	UPROPERTY(EditAnywhere, Category = "Block", meta = (AllowPrivateAccess = "true"))
		FName BlockChannel;

	bool CanBlocking;

	ACharacter* Character;
	UCharacterMovementComponent* CharacterMovement;

public:
	void Init();
	void BlockActive(bool IsOn);
private:
	void BlockOtherActor(float delta);
};
