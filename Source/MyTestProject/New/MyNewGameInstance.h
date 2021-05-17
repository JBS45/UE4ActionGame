// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyNewGameInstance.generated.h"

class UDataTable;

UCLASS()
class MYTESTPROJECT_API UMyNewGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyNewGameInstance();
	
private:
	virtual void Init() override;

private:
	UDataTable* TotalCommandTable;
	UDataTable* WeaponDataTable;
	UDataTable* PlayerData;
public:
	FORCEINLINE UDataTable* GetTotalCommandTable();
	FORCEINLINE UDataTable* GetWeaponDataTable();
	FORCEINLINE UDataTable* GetPlayerDataTable();
};
