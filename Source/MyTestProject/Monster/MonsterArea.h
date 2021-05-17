// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseStatus.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BaseMonster.h"
#include "MonsterSpawner.h"
#include "Engine/TargetPoint.h"
#include "MonsterArea.generated.h"

UCLASS()
class MYTESTPROJECT_API AMonsterArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:

	//Todo : Make MonsterSpawner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Area", meta = (AllowPrivateAccess = "true"))
		USphereComponent* Area;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		AMonsterSpawner* Spawner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseMonster> SpawnMonsterType;
	FMonsterStatus  SpawnMonsterStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		bool OnceSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		float MaxMonsterCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		float CurrentMonsterCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		float MonsterRegenCoolTime = 2.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		float MonsterRegenTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		TArray<ABaseMonster*> Monsters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		ATargetPoint* SpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		float SpawnRange = 200;;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		TArray<ATargetPoint*> PatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detect", meta = (AllowPrivateAccess = "true"))
		bool IsPlayerInRange = false;

private:
	void SpawnUpdate(float DeltaSeconds);
	void Spawn();
	void Reset();
public:
	float GetAreaRange();
	bool GetIsPlayerInRange();
	TArray<ATargetPoint*> GetPatrolPoint();
	TArray<ABaseMonster*> GetMonsters();
	void Notify(ACharacter* target);
};
