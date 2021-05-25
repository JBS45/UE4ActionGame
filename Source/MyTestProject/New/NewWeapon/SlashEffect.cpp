// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashEffect.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ASlashEffect::ASlashEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Effect = CreateDefaultSubobject<UNiagaraComponent>("VFX");
	Sound = CreateDefaultSubobject<UAudioComponent>("Sound");
}

// Called when the game starts or when spawned
void ASlashEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlashEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

