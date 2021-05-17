// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "MyTestProject.h"
#include "BaseStatus.h"
#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

/**
 *
 */
UCLASS()
class MYTESTPROJECT_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

		ABaseGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class ABasePlayerController* PlayerControl;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TMap<uint8, FMonsterStatus> MonsterData;
public:
	FMonsterStatus GetMonsterData(uint8 MonsterId);
};
