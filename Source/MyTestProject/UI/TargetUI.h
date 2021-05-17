// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetUI.generated.h"

/**
 *
 */
UCLASS()
class MYTESTPROJECT_API UTargetUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void PlayAnim();
private:
	UPROPERTY(meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"))
		class UWidgetAnimation* LockOn;
};
