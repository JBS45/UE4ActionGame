// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeWidget.generated.h"



class UImage;

DECLARE_DELEGATE(FVoidDelegate);

UCLASS()
class MYTESTPROJECT_API UFadeWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;
private:
	UPROPERTY(EditAnywhere, Category = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* MaterialOrigin;

	UImage* FadeImage;
	UMaterialInstanceDynamic* MaterialDynamic;

	bool IsFadeOut;
	bool IsFadeIn;

	float DissolveValue =0.0;
public:
	FVoidDelegate PlayerDeadDel;
	FVoidDelegate PlayerSpawnDel;
public:
	void BeginFadeOut();
	void TickFadeOut(float delta);
	void EndFadeOut();

	void BeginFadeIn();
	void TickFadeIn(float delta);
	void EndFadeIn();
};
