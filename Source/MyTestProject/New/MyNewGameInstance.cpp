// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewGameInstance.h"
#include "Engine/DataTable.h"
#include "NewMonsterComponents/NewMonsterStruct.h"

UMyNewGameInstance::UMyNewGameInstance() {

}

void UMyNewGameInstance ::Init() {

	TotalCommandTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Character/Command/TotalCommands.TotalCommands'"), NULL, LOAD_None, NULL);
	WeaponDataTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Weapon/WeaponDataTable.WeaponDataTable'"), NULL, LOAD_None, NULL);
	PlayerData = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Character/PlayerStatus.PlayerStatus'"), NULL, LOAD_None, NULL);

	MonsterData = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Monster/MonstersData.MonstersData'"), NULL, LOAD_None, NULL);
	TArray<FNewMonsterData*> DataOut;
	MonsterData->GetAllRows(TEXT("Load Monster Data"), DataOut);
	for (auto data : DataOut) {
		MonsterDataMap.Add(data->MonsterID, data);
	}
}

UDataTable*  UMyNewGameInstance::GetTotalCommandTable() {
	return TotalCommandTable;
}
UDataTable*  UMyNewGameInstance::GetWeaponDataTable() {
	return WeaponDataTable;
}
UDataTable* UMyNewGameInstance::GetPlayerDataTable() {
	return PlayerData;
}
FNewMonsterData* UMyNewGameInstance::GetMonsterData(uint8 id) {
	return MonsterDataMap[id];
}