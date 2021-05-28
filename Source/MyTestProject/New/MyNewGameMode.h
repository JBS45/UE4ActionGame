// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyNewGameMode.generated.h"

class AMyNewPlayerController;
class UGameClearUI;

UCLASS()
class MYTESTPROJECT_API AMyNewGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMyNewGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController * NewPlayer) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float Record = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Clear", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UGameClearUI> ClearUI;
public:
	void Clear(float Time);
	void Respawn(AMyNewPlayerController& Controller);
};
