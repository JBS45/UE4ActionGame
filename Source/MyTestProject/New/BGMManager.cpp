// Fill out your copyright notice in the Description page of Project Settings.


#include "BGMManager.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// Sets default values
ABGMManager::ABGMManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MainAudio = CreateDefaultSubobject< UAudioComponent>("BGMAudio");
	MainAudio->bAllowSpatialization = false;
	Tags.Add(FName("BGM"));
}

// Called when the game starts or when spawned
void ABGMManager::BeginPlay()
{
	Super::BeginPlay();
	PlayBossBGM(false);
}

// Called every frame
void ABGMManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABGMManager::PlayBossBGM(bool IsBossBattle) {
	auto Sound = IsBossBattle ? Boss : Normal;
	MainAudio->SetSound(Sound);
	MainAudio->Play();
}

void ABGMManager::ControlVolume(float value) {
	MainAudio->SetVolumeMultiplier(value);
}