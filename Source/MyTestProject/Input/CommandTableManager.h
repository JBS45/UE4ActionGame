// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "Components/ActorComponent.h"
#include "../Components/MyInterface.h"
#include "CommandTableManager.generated.h"


class UAnimMontage;
DECLARE_DELEGATE_OneParam(FVoidDelegateChain, TArray<FChainAction>);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UComandTableManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComandTableManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		EWeaponType CurrentWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		ECommandName CurrentCommandName;
private:
	TMap<EWeaponType, TMap<ECommandName, FChainAction>> TotalCommands;
	TMap<ECommandName, FChainAction>* CurrentCommands;
	TArray<ECommandName> EnableAction;
	TArray<IChainInterface*> ChainObservers;
private:
	bool IsSpecial = false;
	void LoadCommandTable(FString ref);
	UAnimMontage* FindAnmation(ECommandName actionName);
	void Notify(ECommandName currentActionName);
public:
	FORCEINLINE bool ValidCurrentCommandName() const;
	FORCEINLINE void SetSpecial(bool IsOn);
	FORCEINLINE FChainAction FindAction(ECommandName actionName);
	void SetCurrentCommandName(ECommandName commandName);
	TArray<ECommandName> FindEnableAction(ECommandName currentActionName);
	void ChangeCommandTable(EWeaponType weapon);
	void Attach(IChainInterface* newObserver);
};