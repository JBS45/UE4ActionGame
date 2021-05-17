// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageText.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "../Player/BasePlayerController.h"

void UDamageText::NativeConstruct() {
	Super::NativeConstruct();
}

void UDamageText::PlayAnim() {
	IsEnable = false;
	PlayAnimation(Floating);
}

void  UDamageText::Init() {
	TextBox = Cast<UTextBlock>(GetWidgetFromName("DamageText"));
	TextBox->SetVisibility(ESlateVisibility::Hidden);
	FinishAnim.BindDynamic(this, &UDamageText::FinshEvent);
	BindToAnimationFinished(Floating, FinishAnim);
}

void UDamageText::UseDamagetText(class ABasePlayerController* control, FVector worldlocation, float damage, bool IsWeak) {
	auto text = FString::FromInt(FMath::RoundToInt(damage));
	TextBox->SetText(FText::FromString(text));
	FLinearColor TextColor = FLinearColor::White;

	if (IsWeak) {
		TextColor = FLinearColor(0.4, 0.2, 0.0, 1.0);
	}
	this->SetColorAndOpacity(TextColor);

	FVector2D ScreenPos = FVector2D::ZeroVector;

	if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(control, worldlocation, ScreenPos, false)) {
		SetRenderTranslation(ScreenPos);
		TextBox->SetVisibility(ESlateVisibility::Visible);
	}
	PlayAnim();
}
void UDamageText::FinshEvent() {
	IsEnable = true;
	TextBox->SetVisibility(ESlateVisibility::Hidden);
}