// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWidget.h"
#include "CommandWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TargetUI.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "DamageText.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "../New/PlayerCharacterInterface/MyNewInputInterface.h"
#include "../New/PlayerCharacter/MyNewPlayerController.h"
#include "../New/NewMonster/MyNewBaseMonster.h"
#include "Components/Overlay.h"
#include "PotionWidget.h"
#include "FadeWidget.h"


void UBaseWidget::NativeConstruct() {
	Super::NativeConstruct();
	PlayerStatusUI = Cast<UUserWidget>(GetWidgetFromName("BaseBar"));
	CommandBoxUI = Cast<UVerticalBox>(GetWidgetFromName("CommandBox"));
	HPBar = Cast<UProgressBar>(PlayerStatusUI->GetWidgetFromName("HpBar"));
	StaminaBar = Cast<UProgressBar>(PlayerStatusUI->GetWidgetFromName("StaminaBar"));
	TargetMark = Cast<UTargetUI>(GetWidgetFromName("TargetMarkUI"));
	PotionImage = Cast<UPotionWidget>(GetWidgetFromName("Potion"));
	MainPanel = Cast<UCanvasPanel>(GetRootWidget());

	if (MainPanel != nullptr) {
		WidgetTree->RootWidget = MainPanel;
	}
}

void UBaseWidget::Init() {

	for (auto command : CommandBoxUI->GetAllChildren()) {
		CommandBars.Add(Cast<UCommandWidget>(command));
	}
	TargetMark->SetVisibility(ESlateVisibility::Hidden);

	if (DamageTextPool.Num() > 0) {
		for (int i = 0; i < PoolLimit; ++i) {
			DamageTextPool[i]->Init();
		}
	}
	else {
		for (int i = 0; i < PoolLimit; ++i) {
			auto Damage = WidgetTree->ConstructWidget<UDamageText>(DamageText);
			Damage->Init();
			MainPanel->AddChild(Damage);
			DamageTextPool.Add(Damage);
		}
	}
}

void UBaseWidget::TraceTarget(AMyNewPlayerController* control,AMyNewBaseMonster* target) {
	FVector2D ScreenPos;
	if (target != nullptr) {
		TargetMark->SetVisibility(ESlateVisibility::Visible);
		TargetMark->PlayAnim();
		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(control, target->GetMesh()->GetSocketLocation("TargetMark"), ScreenPos, false)) {
			TargetMark->SetRenderTranslation(ScreenPos);
		}
	}
	else {
		TargetMark->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UBaseWidget::UseDamageText(FVector worldlocation, int32 damage, bool IsWeak) {
	for (auto element : DamageTextPool) {
		if (element->GetIsEnable()) {
			element->UseDamagetText(GetOwningPlayer(), worldlocation, damage, IsWeak);
			break;
		}
	}
}
void UBaseWidget::NotifyStatus(float maxhp, float currhp, float maxstatmina, float currstamina) {
	float HPRate = currhp / maxhp;
	float StaminaRate = currstamina / maxstatmina;
	HPBar->SetPercent(HPRate);
	StaminaBar->SetPercent(StaminaRate);
}

void UBaseWidget::NotifyCommand(TArray<FNewChainAction> chain) {
	TArray<FNewChainAction> VisibleChain;

	for (auto element : chain) {
		if (element.IsGUIVisible) {
			VisibleChain.Add(element);
		}
	}
	for (auto bar : CommandBars) {
		bar->HideWidget();
	}
	for (int i = 0; i < VisibleChain.Num(); ++i) {
		CommandBars[i]->SetCommand(VisibleChain[i].AttackName, VisibleChain[i].MoveCommand, VisibleChain[i].ActionCommand);
	}
}
void UBaseWidget::ChangeWeapon(const ENewWeaponType weapon) {
	auto Axe = PlayerStatusUI->GetWidgetFromName("Axe");
	auto Katana = PlayerStatusUI->GetWidgetFromName("Katana");
	auto Dual = PlayerStatusUI->GetWidgetFromName("Dual");

	switch (weapon)
	{
	case ENewWeaponType::E_DUAL:
		Dual->SetVisibility(ESlateVisibility::Visible);
		Katana->SetVisibility(ESlateVisibility::Hidden);
		Axe->SetVisibility(ESlateVisibility::Hidden);
		break;
	case ENewWeaponType::E_KATANA:
		Dual->SetVisibility(ESlateVisibility::Hidden);
		Katana->SetVisibility(ESlateVisibility::Visible);
		Axe->SetVisibility(ESlateVisibility::Hidden);
		break;
	case ENewWeaponType::E_AXE:
		Dual->SetVisibility(ESlateVisibility::Hidden);
		Katana->SetVisibility(ESlateVisibility::Hidden);
		Axe->SetVisibility(ESlateVisibility::Visible);
		break;
	default:
		break;
	}
}

void UBaseWidget::UpdatePotion(float remain, float coolTime) {
	PotionImage->UpdatePotion(remain, coolTime);
}
