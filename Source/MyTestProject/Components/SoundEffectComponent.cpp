// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundEffectComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
USoundEffectComponent::USoundEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	SoundValue = 0.5f;
	AttenuationDistance = 1800.0f;
	InnerDistance = 1000.0f;

	AttenuationSetting.BinauralRadius = InnerDistance;
	AttenuationSetting.FocusVolumeAttenuation = AttenuationDistance;
	// ...
	Audio = CreateDefaultSubobject<UAudioComponent>("Sound");
	Audio->bOverrideAttenuation = true;
	Audio->VolumeMultiplier = SoundValue;
	Audio->AdjustAttenuation(AttenuationSetting);

	ComponentTags.Add("SoundEffect");
}


// Called when the game starts
void USoundEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CompOwenr = GetOwner();
	Audio->AttachToComponent(CompOwenr->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	ResetAttenuation();
}


// Called every frame
void USoundEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void USoundEffectComponent::PlaySE() {
	Audio->SetSound(PlaySound);
	Audio->Play();
}
void USoundEffectComponent::SetAttenuation(float attenuation, float innner) {
	if (Audio->bOverrideAttenuation) {
		AttenuationSetting.BinauralRadius = innner;
		AttenuationSetting.FocusVolumeAttenuation = attenuation;
		Audio->AdjustAttenuation(AttenuationSetting);
	}
}

void USoundEffectComponent::ResetAttenuation() {
	if (Audio->bOverrideAttenuation) {
		AttenuationSetting.BinauralRadius = InnerDistance;
		AttenuationSetting.FocusVolumeAttenuation = AttenuationDistance;
		Audio->AdjustAttenuation(AttenuationSetting);
	}
}

void USoundEffectComponent::AdjustSound(float value) {
	SoundValue = value;
	Audio->VolumeMultiplier = SoundValue;
}