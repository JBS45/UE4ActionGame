// Fill out your copyright notice in the Description page of Project Settings.



#include "CommandWidget.h"
#include "../BaseStatus.h"


UCommandWidget::UCommandWidget(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
	LoadImage();
}
void UCommandWidget::NativeConstruct() {
	Super::NativeConstruct();
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Move1")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Plus")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Action")));
	CommandImages.Add(Cast<UImage>(GetWidgetFromName("Key")));
	CommandName = Cast<UTextBlock>(GetWidgetFromName("CommandName"));
	KeyName = Cast<UTextBlock>(GetWidgetFromName("KeyText"));


}

void UCommandWidget::HideWidget() {
	for (auto image : CommandImages) {
		image->SetVisibility(ESlateVisibility::Hidden);
	}
	CommandName->SetVisibility(ESlateVisibility::Hidden);
	KeyName->SetVisibility(ESlateVisibility::Hidden);

}
void UCommandWidget::SetCommand(FString commandName, int32 move, ENewActionKey action) {
	CommandName->SetVisibility(ESlateVisibility::Visible);
	CommandName->SetText(FText::FromString(commandName));

	SetImage(move, action);
}

void UCommandWidget::LoadImage() {

	static ConstructorHelpers::FObjectFinder<UTexture2D> LMBIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/LMB.LMB'"));
	if (LMBIMAGE.Succeeded())
	{
		ImageArray.Add(LMBIMAGE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> RMBIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/RMB.RMB'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(RMBIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> RLMB(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/LMB_RMB.LMB_RMB'"));
	if (RLMB.Succeeded())
	{
		ImageArray.Add(RLMB.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> SPACEBAR(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/SpaceBar.SpaceBar'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(SPACEBAR.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> FRONTIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Up.Up'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(FRONTIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> BACKIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Down.Down'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(BACKIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> LEFTIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Left.Left'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(LEFTIMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> RIGHTIMAGE(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Right.Right'"));
	if (RMBIMAGE.Succeeded())
	{
		ImageArray.Add(RIGHTIMAGE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> PLUS(TEXT("Texture2D'/Game/UI/CharaterStatus/CommandBar/Plus.Plus'"));
	if (PLUS.Succeeded())
	{
		PlusImage = PLUS.Object;
	}


}
void UCommandWidget::SetImage(int32 move, ENewActionKey action) {
	if (action == ENewActionKey::E_E) {
		CommandImages[3]->SetVisibility(ESlateVisibility::Visible);
		KeyName->SetVisibility(ESlateVisibility::Visible);
		KeyName->SetText(FText::FromString("E"));
	}
	else if (action == ENewActionKey::E_TAB) {
		CommandImages[3]->SetVisibility(ESlateVisibility::Visible);
		KeyName->SetVisibility(ESlateVisibility::Visible);
		KeyName->SetText(FText::FromString("TAB"));
	}
	else {
		CommandImages[2]->SetVisibility(ESlateVisibility::Visible);
		CommandImages[2]->Brush.SetResourceObject(SelectImage(action));
	}

	auto MoveImage = SelectImage(move);
	if (MoveImage == nullptr || action == ENewActionKey::E_SPACEBAR) {
		CommandImages[0]->SetVisibility(ESlateVisibility::Hidden);
		CommandImages[1]->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		CommandImages[0]->SetVisibility(ESlateVisibility::Visible);
		CommandImages[0]->Brush.SetResourceObject(MoveImage);


		CommandImages[1]->SetVisibility(ESlateVisibility::Visible);
		CommandImages[1]->Brush.SetResourceObject(PlusImage);
	}
	
}
UTexture2D* UCommandWidget::SelectImage(int32 move) {
	UTexture2D* Result = nullptr;

	if (move == (int32)EMoveKeyFlag::E_NONE) {
		return Result;
	}
	else if ((move&(int32)EMoveKeyFlag::E_FORWARD) != 0) {
		Result = ImageArray[4];
	}
	else if ((move&(int32)EMoveKeyFlag::E_BACKWARD) != 0) {
		Result = ImageArray[5];
	}
	else if ((move&(int32)EMoveKeyFlag::E_LEFT) != 0) {
		Result = ImageArray[6];
	}
	else if ((move&(int32)EMoveKeyFlag::E_RIGHT) != 0) {
		Result = ImageArray[7];
	}

	return Result;
}
UTexture2D* UCommandWidget::SelectImage(ENewActionKey action) {
	UTexture2D* Result = nullptr;

	switch (action) {
	case ENewActionKey::E_LEFTCLICK:
		Result = ImageArray[0];
		break;
	case ENewActionKey::E_RIGHTCLICK:
		Result = ImageArray[1];
		break;
	case ENewActionKey::E_LEFTRIGHTCLICK:
		Result = ImageArray[2];
		break;
	case ENewActionKey::E_SPACEBAR:
		Result = ImageArray[3];
		break;
	}

	return Result;
}