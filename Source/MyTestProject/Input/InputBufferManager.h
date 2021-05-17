// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "../MyTestProject.h"
#include "Engine/DataTable.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h"
#include "../Player/BaseCharAnimInstance.h"
#include "InputBufferManager.generated.h"

#define COMMAND_BASE_STATE "Base"
#define COMMAND_DRAWWEAPON "Draw"
#define COMMAND_PUTUPWEAPON "PutUp"
#define COMMAND_SPECIAL "Special"
#define COMMAND_EVADE "Evade"
#define INPUT_BUFFER_DELAY 0.15f

class UComandTableManager;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UInputBufferManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInputBufferManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Input")
		UComandTableManager* CommandTable;

	UPROPERTY(VisibleAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
		TArray<FMoveInputInfo> MoveInputBuffer;
	UPROPERTY(VisibleAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
		TArray<FActionInputInfo> ActionInputBuffer;

	bool bPressedW = false;
	bool bPressedS = false;
	bool bPressedD = false;
	bool bPressedA = false;

	bool StateSpecial = false;

	float BufferCheckTime = 0.0f;
public:
	FVoidDelegateAnimMontage PlayAnimEventDel;
	FVoidDelegate PlayGetUpAnim;
	FCheckCanEvadStamina EvadeConditionDel;
private:
	UPROPERTY(VisibleAnywhere, Category = "State")
		EWeaponType CurrentWeapon;
	UPROPERTY(VisibleAnywhere, Category = "State")
		ECharacterState CurrentState;
	UPROPERTY(VisibleAnywhere, Category = "State")
		ECommandName CurrentActionName;
	UPROPERTY(VisibleAnywhere, Category = "State")
		ECommandName ResetActionName;
	UPROPERTY(VisibleAnywhere, Category = "BufferState")
		bool CanBufferInput = true;

public:
	FORCEINLINE void SetCommandTable(UComandTableManager* table);
	FORCEINLINE void InputBufferOpen();
	FORCEINLINE void InputBufferClose();
	FORCEINLINE void SetPressedW(bool value);
	FORCEINLINE void SetPressedS(bool value);
	FORCEINLINE void SetPressedD(bool value);
	FORCEINLINE void SetPressedA(bool value);

	void AddInputToInputBuffer(FMoveInputInfo input);
	void AddInputToInputBuffer(FActionInputInfo input);

	void ChangeCharacterState(ECharacterState state);
	void ChangeWeapon(EWeaponType weapon);

	void ReadyForNextAction(ECommandName newActionName);

	void SetCurrentAction(ECommandName actionName);
	void ChainReset();
	void PutUpWeapon();
	void InputBufferChecker();

	void SpecialOn();
	void SpecialOff();


private:
	bool CheckCommand();
	bool MoveCommnadCheck(TArray<EMoveKey> Command, float& LastInput);
	bool ActionCommnadCheck(TArray<EActionKey> Command, float& FirstInput);
	void ActionBufferCheck();
	void MoveBufferCheck();
	void MoveAddInput(bool Pressed, FMoveInputInfo input);
	void PlayAnimation(ECommandName actionName);
	void CharacterStateTick();
	bool EvadeOrStep();
	bool EvadeAction(ECommandName actionName);
	void ClearInputBuffer();
};
