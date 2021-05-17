// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "../MyTestProject.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../New/PlayerCharacterInterface/MyNewInputInterface.h"
#include "CommandWidget.generated.h"

/**
 *
 */
UCLASS()
class MYTESTPROJECT_API UCommandWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCommandWidget(const FObjectInitializer& objectInitializer);
protected:

	virtual void NativeConstruct() override;

	TArray<UImage*> CommandImages;
	UTextBlock* CommandName;
	UTextBlock* KeyName;

	TArray<UTexture2D*> ImageArray;
	UTexture2D* PlusImage;

public:
	void HideWidget();
	void SetCommand(FString commandName, int32 move, ENewActionKey action);

private:
	void LoadImage();
	void SetImage(int32 move, ENewActionKey action);
	UTexture2D* SelectImage(int32 move);
	UTexture2D* SelectImage(ENewActionKey action);
};