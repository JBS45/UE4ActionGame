// Fill out your copyright notice in the Description page of Project Settings.


#include "FadeWidget.h"
#include "Components/Image.h"


void UFadeWidget::NativeConstruct() {
	Super::NativeConstruct();
	
	FadeImage = Cast<UImage>(GetWidgetFromName("FadePanel"));
	DissolveValue = 2.0f;
	if (MaterialOrigin != nullptr) {
		MaterialDynamic = UMaterialInstanceDynamic::Create(MaterialOrigin, this);
		MaterialDynamic->SetScalarParameterValue("Dissolve", DissolveValue);
		FadeImage->SetBrushFromMaterial(MaterialDynamic);
	}
	BeginFadeIn();
}

void UFadeWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (IsFadeOut) {
		TickFadeOut(InDeltaTime);
	}
	else if (IsFadeIn) {
		TickFadeIn(InDeltaTime);
	}
}

void UFadeWidget::BeginFadeOut() {
	IsFadeOut = true;
}
void UFadeWidget::TickFadeOut(float delta) {
	DissolveValue += (0.5f*delta);
	if (DissolveValue >= 2.0f) {
		IsFadeOut = false;
		DissolveValue = 2.0f;
		EndFadeOut();
	}
	MaterialDynamic->SetScalarParameterValue("Dissolve", DissolveValue);

}
void UFadeWidget::EndFadeOut() {
	PlayerDeadDel.ExecuteIfBound();
}

void UFadeWidget::BeginFadeIn() {
	IsFadeIn = true;
}
void UFadeWidget::TickFadeIn(float delta) {
	DissolveValue -= (2.0f*delta);
	if (DissolveValue <= 0.0f) {
		IsFadeIn = false;
		DissolveValue = 0.0f;
		EndFadeIn();
	}
	MaterialDynamic->SetScalarParameterValue("Dissolve", DissolveValue);

}
void UFadeWidget::EndFadeIn() {
	PlayerSpawnDel.ExecuteIfBound();
}