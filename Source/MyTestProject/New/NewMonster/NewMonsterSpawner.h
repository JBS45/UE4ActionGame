// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewMonsterSpawner.generated.h"

class USphereComponent;
class AMyNewBaseMonster;
class AMyNewCharacter;
struct FNewMonsterData;

UCLASS()
class MYTESTPROJECT_API ANewMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewMonsterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	UFUNCTION()
		void OnPlayerInRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnPlayerOutRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere, Category = "Area", meta = (AllowPrivateAccess = "true"))
		USphereComponent* MonsterArea;
	UPROPERTY(VisibleAnywhere, Category = "Area", meta = (AllowPrivateAccess = "true"))
		AMyNewCharacter* Player;
	bool IsPlayerInArea;

	UPROPERTY(EditAnywhere, Category = "Monster",meta = (AllowPrivateAccess = "true"))
		TArray<uint8> MonsterID;
	UPROPERTY(VisibleAnywhere, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		TArray<AMyNewBaseMonster*> MonstersInArea;

	TMap<uint8, FNewMonsterData*> MonsterData;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		bool IsOnlyOnceSpawn;
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		uint8 MaxMonsterCountInArea;
	UPROPERTY(VisibleAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
		uint8 CurrMonsterCountInArea;

private:
	void SetUpData();
	void Spawn(const FNewMonsterData& data);

	void CheckPlayerState();
	void CheckMonsterState();
public:
	void SpawnMonster();
	FVector FindPatrolPoint();
	FORCEINLINE bool GetIsPlayerInArea() { return IsPlayerInArea; }
};
