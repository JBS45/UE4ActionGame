// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyNewGameState.generated.h"


class AMyNewPlayerController;

UCLASS()
class MYTESTPROJECT_API AMyNewGameState : public AGameState
{
	GENERATED_BODY()
public:
	AMyNewGameState();
	virtual void Tick(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "Kill", meta = (AllowPrivateAccess = "true"))
		uint8 TargetID;
	UPROPERTY(EditAnywhere, Category = "Kill", meta = (AllowPrivateAccess = "true"))
		uint8 TargetCount;
	UPROPERTY(VisibleAnywhere, Category = "Kill", meta = (AllowPrivateAccess = "true"))
		TMap<uint8, uint8> KillMonsterCount;

	UPROPERTY(VisibleAnywhere, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float Timer = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Dead", meta = (AllowPrivateAccess = "true"))
		uint8 DeadCount = 0;
public:
	void Notify(uint8 MonsterID);
	void Clear();
	void PlayerDead(AMyNewPlayerController& controller);
	void Respawn(AMyNewPlayerController& controller);
};
