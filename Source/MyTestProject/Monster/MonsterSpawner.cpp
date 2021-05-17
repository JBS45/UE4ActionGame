// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "NavigationSystem.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ABaseMonster* AMonsterSpawner::SpawnMonster(TSubclassOf<ABaseMonster> monsterType, FVector origin, float areaRange) {
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) {
		return nullptr;
	}
	FNavLocation NextLocation;

	NavSystem->GetRandomPointInNavigableRadius(origin, areaRange, NextLocation);
	FVector location = NextLocation.Location + FVector(0, 0, 80);
	FRotator rotator = FRotator::ZeroRotator;
	auto result = GetWorld()->SpawnActor<ABaseMonster>(monsterType, location, rotator);

	return result;
}