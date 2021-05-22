// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPotionWidget::NativeConstruct() {
	PotionImage = Cast<UProgressBar>(GetWidgetFromName("Potion"));
	CoolTimeText = Cast<UTextBlock>(GetWidgetFromName("Cooltime"));
}

void UPotionWidget::UpdatePotion(float remain, float coolTime) {
	PotionImage->SetPercent(1-(remain / coolTime));
	if (remain > 0) {
		CoolTimeText->SetVisibility(ESlateVisibility::Visible);
		CoolTimeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"),remain)));
	}
	else {
		CoolTimeText->SetVisibility(ESlateVisibility::Hidden);
	}
}