// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PlayerCharacterInterface/MyNewInputInterface.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "MyNewCommandTable.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTESTPROJECT_API UMyNewCommandTable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyNewCommandTable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewWeaponType CurrentWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewCommandName CurrentCommandName;
private:
	TMap<ENewWeaponType, TMap<ENewCommandName, FNewChainAction>> TotalCommands;
	TMap<ENewCommandName, FNewChainAction>* CurrentCommands;
	TArray<ENewCommandName>* EnableAction;

	bool IsInitCompleted;

	TArray<IChangeCommand*> Observers;
public:
	void LoadCommandTable();


	void SetCurrentCommandName(const ENewCommandName commandName);
	void ChangeCommandTable(const ENewWeaponType weapon, const ENewCommandName InitCommandName = ENewCommandName::E_BASE);
	TArray<FNewChainAction> MakeEnableChainAction();
	void Attach(IChangeCommand* Observer);
private:
	void Notify();
public:
	FORCEINLINE UAnimMontage* FindAnimation(ENewCommandName actionName) { 
		return CurrentCommands->Find(actionName)->ActionMontage;
	};
	FORCEINLINE FNewChainAction FindAction(const ENewCommandName actionName) {
		return *CurrentCommands->Find(actionName); 
	};
	FORCEINLINE TArray<ENewCommandName>* FindEnableAction(const ENewCommandName currentActionName) { 
		return &(CurrentCommands->Find(currentActionName)->EnableChainAction); 
	};
};
