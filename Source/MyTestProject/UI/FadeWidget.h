// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeWidget.generated.h"



class UImage;

UCLASS()
class MYTESTPROJECT_API UFadeWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

	UImage* FadeImage;

	bool FadeOut;
	bool FadeIn;

public:
	void BeginFadeOut();
	void TickFadeOut(float delta);
	void EndFadeOut();

	void BeginFadeIn();
	void TickFadeIn(float delta);
	void EndFadeIn();
};
