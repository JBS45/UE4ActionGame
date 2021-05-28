// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleScreen.generated.h"

class UButton;

UCLASS()
class MYTESTPROJECT_API UTitleScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
private:
	FName LevelName;

	UButton* StartBtn;
	UButton* EndBtn;
private:
	UFUNCTION()
	void GoToMainLevel();
	UFUNCTION()
	void QuitGame();
};
