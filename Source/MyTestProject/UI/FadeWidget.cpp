// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeWidget.h"
#include "Components/Image.h"


void UFadeWidget::NativeConstruct() {
	Super::NativeConstruct();
	
	FadeImage = Cast<UImage>(GetWidgetFromName("FadePanel"));
}

void UFadeWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime)
}