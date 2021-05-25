// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlashEffect.generated.h"

class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class MYTESTPROJECT_API ASlashEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlashEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "VFX", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* Effect;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		UAudioComponent* Sound;
};
