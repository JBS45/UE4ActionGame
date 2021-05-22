// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "Blueprint/UserWidget.h"
#include "../BaseStatus.h"
#include "../BaseEnum.h"
#include "../Components/MyInterface.h"
#include "../New/PlayerCharacterInterface/NewUnrealInterface.h"
#include "BaseWidget.generated.h"

class ABasePlayerController;

UCLASS()
class MYTESTPROJECT_API UBaseWidget : public UUserWidget, public IUpdateStatus, public IChangeCommand
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	void SetCameraLockOn(bool IsOn);
	void TraceTarget(ABasePlayerController* control, class ABaseMonster* target);
	void UseDamageText(FVector worldlocation, int32 damage, bool IsWeak);
	void ChangeWeapon(const ENewWeaponType weapon);
	void UpdatePotion(float remain, float coolTime);
	virtual void NotifyStatus(float maxhp, float currhp, float maxstatmina, float currstamina)  override;
	virtual void NotifyCommand(TArray<FNewChainAction> actions) override;
private:
	class UCharacterStatusManager* CurrentCharacterStatus;
	class UCommandTableManager* CommandTable;


	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UUserWidget* PlayerStatusUI;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UVerticalBox* CommandBoxUI;
	UPROPERTY()
		TArray<class UCommandWidget*> CommandBars;
	UPROPERTY()
		class UPotionWidget* PotionImage;
	UPROPERTY()
		class UProgressBar* HPBar;
	UPROPERTY()
		class UProgressBar* StaminaBar;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UTargetUI* TargetMark;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UDamageText> DamageText;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<class UDamageText*> DamageTextPool;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		int32 PoolLimit = 50;
};
