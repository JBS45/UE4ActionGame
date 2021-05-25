// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PlayerCharacterInterface/MyNewInputInterface.h"
#include "../PlayerCharacterInterface/MyNewPlayerinterface.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "MyNewInputBuffer.generated.h"

class AMyNewPlayerController;
class AMyNewCharacter;
class UMyNewCommandTable;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UMyNewInputBuffer : public UActorComponent, public IResetInputBuffer,public IChangeCommand
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyNewInputBuffer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	const float BufferDelay = 0.1f;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		EInputBufferState CurrentInputBufferState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewWeaponType CurrentWeaponType;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewPlayerState CurrentPlayerState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewActionState CurrentActionState;

	AMyNewPlayerController* CurrentController;
	AMyNewCharacter* CurrentCharacter;

	UPROPERTY(EditAnywhere, Category = "input", meta = (AllowPrivateAccess = "true"))
		TArray<FMoveKeyState> MoveInputBuffer;
	FMoveKeyState SaveKeyState;
	UPROPERTY(EditAnywhere, Category = "input", meta = (AllowPrivateAccess = "true"))
		TArray<FActionKeyState> ActionInputBuffer;

	FMoveKeyState LastBeforeAction;
	float CheckTimer;
	bool TimerFlag;

	bool IsSprintFlag;

	UPROPERTY(VisibleAnywhere, Category = "Command", meta = (AllowPrivateAccess = "true"))
		UMyNewCommandTable* CommandTable;
	UPROPERTY(VisibleAnywhere, Category = "Command", meta = (AllowPrivateAccess = "true"))
		ENewCommandName CurrentCommandName;
	UPROPERTY(VisibleAnywhere, Category = "Command", meta = (AllowPrivateAccess = "true"))
		ENewCommandName ResetCommandName;
	UPROPERTY(VisibleAnywhere, Category = "Command", meta = (AllowPrivateAccess = "true"))
		TArray<FNewChainAction> EnableActions;

public:
	void InputBufferStateChange(EInputBufferState state);

	void MoveForward(float value);
	void MoveRight(float value);
	void CameraLookUp(float value);
	void CameraTurn(float value);

	void PressShift();
	void ReleaseShift();

	void ChangePlayerState(const ENewPlayerState state);
	void ChangeActionState(const ENewActionState state);
	void ChangeWeaponState(const ENewWeaponType weapon);

	virtual void NotifyReset() override;
	virtual void NotifyCommand(TArray<FNewChainAction> actions) override;

	void SetCurrentCharacter(AMyNewCharacter* character);
	void ClearCurrentCharacter();
private:
	void InputBufferStateProcess(float delta);
	void SaveMoveKey();
	void CheckAction(float delta);
	void ActiveCommand();
	void ActivateCommand(const ENewCommandName CommandName);
	void PlayAnimation(UAnimMontage* montage);
	ENewWeaponType GetNextWeapon();
	FActionKeyState Click(const ENewActionKey key);

public:

	FORCEINLINE void SetPlayerController(AMyNewPlayerController* Control) { CurrentController = Control; };
	FORCEINLINE bool GetIsSprintFlag() { return IsSprintFlag; };
	FORCEINLINE UMyNewCommandTable* GetCommandTable() { return CommandTable; };
	FORCEINLINE void LeftClick() { 
		ActionInputBuffer.Add(Click(ENewActionKey::E_LEFTCLICK)); 
	};
	FORCEINLINE void RightClick() { 
		ActionInputBuffer.Add(Click(ENewActionKey::E_RIGHTCLICK)); 
	};
	FORCEINLINE void Evade() {
		if (CurrentCharacter != nullptr) {
			ActionInputBuffer.Add(Click(ENewActionKey::E_SPACEBAR));
		}
	};
	FORCEINLINE void Potion() { 
		if (CurrentCharacter != nullptr) {
		ActionInputBuffer.Add(Click(ENewActionKey::E_C)); 
		} 
	};
	FORCEINLINE void Cast() {
		if (CurrentCharacter != nullptr) {
			ActionInputBuffer.Add(Click(ENewActionKey::E_E)); 
		} 
	};
	FORCEINLINE void WeaponChange() { 
		if (CurrentCharacter != nullptr) {
			ActionInputBuffer.Add(Click(ENewActionKey::E_TAB));
		} 
	};

};
