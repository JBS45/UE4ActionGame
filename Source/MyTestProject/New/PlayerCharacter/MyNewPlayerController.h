// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "../PlayerCharacterComponents/PlayerBuff.h"
#include "MyNewPlayerController.generated.h"


class UMyNewInputBuffer;
class AMyNewCharacter;
class UBaseWidget;
class PotionTimer;

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerStateDel, ENewPlayerState);
DECLARE_MULTICAST_DELEGATE_OneParam(FActionStateDel, ENewActionState);
DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponTypeDel, ENewWeaponType);

UCLASS()
class MYTESTPROJECT_API AMyNewPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyNewPlayerController();

	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Character", meta = (AllowPrivateAccess = "true"))
		AMyNewCharacter* CurrentCharacter;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewPlayerState CurrentPlayerState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewActionState CurrentActionState;
	

	UPROPERTY(VisibleAnywhere, Category = "InputBuffer", meta = (AllowPrivateAccess = "true"))
		UMyNewInputBuffer* InputBuffer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UBaseWidget> HUDWidgetClass;

	UBaseWidget* PlayerHUD;
public:
	void PossessProcess(APawn* pawn);
	void UnPoessessProcess();

	FPlayerStateDel ChangePlayerState;
	FActionStateDel ChangeActionState;
	FWeaponTypeDel ChangeWeaponType;

	TUniquePtr<PotionTimer> PotionCoolTime;
	
	void AttachWidgetToViewport(TSubclassOf<UBaseWidget> widget);

public:
	FORCEINLINE UMyNewInputBuffer* GetInputBuffer() { return InputBuffer; };
	FORCEINLINE void PlayerDead() { ChangePlayerState.Broadcast(ENewPlayerState::E_DEAD); };
	FORCEINLINE void StaminaExhuastion(){if (CurrentPlayerState == ENewPlayerState::E_SPRINT) {ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);}};
	FORCEINLINE UBaseWidget* GetPlayerHUD() { return PlayerHUD; };
	FORCEINLINE TUniquePtr<PotionTimer>& GetPotionCoolTime() {return PotionCoolTime;}
	FORCEINLINE bool GetPotionCanUse() { return PotionCoolTime->GetCanUse(); }
};
