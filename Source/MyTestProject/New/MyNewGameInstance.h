// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "NewMonsterComponents/NewMonsterStruct.h"
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
public:
	void DataLoad();

private:
	UPROPERTY()
		UDataTable* TotalCommandTable;
	UPROPERTY()
		UDataTable* WeaponDataTable;
	UPROPERTY()
		UDataTable* PlayerData;

	UPROPERTY()
		UDataTable* MonsterData;
	UPROPERTY()
		TMap<uint8, FNewMonsterData> MonsterDataMap;

	bool IsInit;
public:
	FORCEINLINE UDataTable& GetTotalCommandTable(){ return *TotalCommandTable; }
	FORCEINLINE UDataTable& GetWeaponDataTable(){ return *WeaponDataTable; }
	FORCEINLINE UDataTable& GetPlayerDataTable(){ return *PlayerData; }
	FORCEINLINE FNewMonsterData& GetMonsterData(uint8 id){ return MonsterDataMap[id]; }
	FORCEINLINE bool GetIsInit() { return IsInit; }
};
