// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewGameInstance.h"
#include "Engine/DataTable.h"
#include "NewMonsterComponents/NewMonsterStruct.h"

UMyNewGameInstance::UMyNewGameInstance() {
	IsInit = false;
}

void UMyNewGameInstance ::Init() {
	Super::Init();
}

void UMyNewGameInstance::DataLoad() {
	if (IsInit == true) return;
	TotalCommandTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Character/Command/TotalCommands.TotalCommands'"), NULL, LOAD_None, NULL);
	if (!IsValid(TotalCommandTable)) return;

	WeaponDataTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Weapon/WeaponDataTable.WeaponDataTable'"), NULL, LOAD_None, NULL);
	if (!IsValid(WeaponDataTable)) return;

	PlayerData = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Character/PlayerStatus.PlayerStatus'"), NULL, LOAD_None, NULL);
	if (!IsValid(PlayerData)) return;

	MonsterData = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Monster/MonstersData.MonstersData'"), NULL, LOAD_None, NULL);
	if (!IsValid(MonsterData)) return;

	TArray<FNewMonsterData*> DataOut;
	MonsterData->GetAllRows(TEXT("Load Monster Data"), DataOut);
	for (auto data : DataOut) {
		MonsterDataMap.Add(data->MonsterID, *data);
	}

	IsInit = true;
}
