// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyNewGameMode.generated.h"

class AMyNewPlayerController;

UCLASS()
class MYTESTPROJECT_API AMyNewGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMyNewGameMode();
private:
	UPROPERTY(VisibleAnywhere, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float Record = 0.0f;

public:
	void Clear(float Time);
	void Respawn(AMyNewPlayerController& Controller);
};
