// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitleScenePlayerControl.generated.h"

class ACameraActor;
class UTitleScreen;


UCLASS()
class MYTESTPROJECT_API ATitleScenePlayerControl : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category = "TargetCamera", meta = (AllowPrivateAccess = "true"))
		ACameraActor* ViewTarget;
	UPROPERTY(EditAnywhere, Category = "TargetCamera", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UTitleScreen> TitleScreen;
};
