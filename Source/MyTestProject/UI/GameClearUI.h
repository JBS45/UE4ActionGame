// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearUI.generated.h"

class UButton;

UCLASS()
class MYTESTPROJECT_API UGameClearUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
private:
	FName TitleName;

	UButton* Button;
private:
	UFUNCTION()
	void GoToTitle();
};
