// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "../BaseEnum.h"
#include "SoundEffectComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API USoundEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USoundEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class AActor* CompOwenr;
	UAudioComponent* Audio;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundCue* PlaySound;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		float AttenuationDistance;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		float InnerDistance;

	float SoundValue;
	FSoundAttenuationSettings AttenuationSetting;

public:
	void PlaySE();
	void SetAttenuation(float attenuation, float innner);
	void ResetAttenuation();
	void AdjustSound(float value);

};