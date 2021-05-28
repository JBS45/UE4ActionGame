// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PlayerCharacterInterface/MyNewInputInterface.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "MyNewCommandTable.generated.h"

USTRUCT(BlueprintType)
struct FCommands {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	TMap<ENewCommandName, FNewChainAction> Chain;
};

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
	/*UPROPERTY()
		typedef TMap<const ENewCommandName, const FNewChainAction> Commands;*/
	UPROPERTY()
		TMap<ENewWeaponType, FCommands> TotalCommands;
	UPROPERTY()
		FCommands CurrentCommands;
	UPROPERTY()
		TArray<ENewCommandName> EnableAction;


	ENewCommandName TestActionName;

	bool IsInitCompleted;

	TArray<IChangeCommand*> Observers;
public:
	void LoadCommandTable();


	void SetCurrentCommandName(const ENewCommandName commandName);
	void ChangeCommandTable(const ENewWeaponType weapon, const ENewCommandName InitCommandName = ENewCommandName::E_BASE);
	TArray<FNewChainAction> MakeEnableChainAction();
	void Attach(IChangeCommand* Observer);
	UAnimMontage* TestFindAnimation(const ENewCommandName actionName);
private:
	void Notify();
	void Check();
private:
	FName CheckName;
public:
	UAnimMontage* FindAnimation(const ENewCommandName actionName) {
		TestActionName = actionName;
		return TestFindAnimation(actionName);
	};
	FNewChainAction FindAction(const ENewCommandName actionName)const {
		return CurrentCommands.Chain[actionName];
	};
	TArray<ENewCommandName>* FindEnableAction(const ENewCommandName currentActionName);
};
