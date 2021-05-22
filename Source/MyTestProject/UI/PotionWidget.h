// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PotionWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class MYTESTPROJECT_API UPotionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	void UpdatePotion(float remain, float coolTime);
private:
	UProgressBar* PotionImage;
	UTextBlock* CoolTimeText;
};
