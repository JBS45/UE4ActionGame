// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "Blueprint/UserWidget.h"
#include "../New/PlayerCharacterInterface/NewUnrealInterface.h"
#include "BaseWidget.generated.h"

class AMyNewPlayerController;
class AMyNewBaseMonster;
class UFadeWidget;
class UPotionWidget;
class UCommandWidget;
class UVerticalBox;
class UProgressBar;
class UTargetUI;
class UDamageText;
class UCanvasPanel;

DECLARE_DELEGATE(FVoidDelegate);

UCLASS()
class MYTESTPROJECT_API UBaseWidget : public UUserWidget, public IUpdateStatus, public IChangeCommand
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	void TraceTarget(AMyNewPlayerController* control, AMyNewBaseMonster* target);
	void UseDamageText(FVector worldlocation, int32 damage, bool IsWeak);
	void ChangeWeapon(const ENewWeaponType weapon);
	void UpdatePotion(float remain, float coolTime);
	virtual void NotifyStatus(float maxhp, float currhp, float maxstatmina, float currstamina)  override;
	virtual void NotifyCommand(TArray<FNewChainAction> actions) override;
	void Init();

private:
	class UCharacterStatusManager* CurrentCharacterStatus;
	class UCommandTableManager* CommandTable;


	UCanvasPanel* MainPanel;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UUserWidget* PlayerStatusUI;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UVerticalBox* CommandBoxUI;
	UPROPERTY()
		TArray<UCommandWidget*> CommandBars;
	UPROPERTY()
		UPotionWidget* PotionImage;
	UPROPERTY()
		UProgressBar* HPBar;
	UPROPERTY()
		UProgressBar* StaminaBar;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UTargetUI* TargetMark;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UDamageText> DamageText;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<UDamageText*> DamageTextPool;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 PoolLimit = 50;
};
