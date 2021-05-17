// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../Monster/BaseMonster.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

UCLASS()
class MYTESTPROJECT_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	ABaseMonster* SpawnMonster(TSubclassOf<ABaseMonster> monsterType, FVector origin, float areaRange);

};
