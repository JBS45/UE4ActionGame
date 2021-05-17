// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewGameInstance.h"
#include "Engine/DataTable.h"

UMyNewGameInstance::UMyNewGameInstance() {

}

void UMyNewGameInstance ::Init() {

	TotalCommandTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Character/Command/TotalCommands.TotalCommands'"), NULL, LOAD_None, NULL);
	WeaponDataTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Weapon/WeaponDataTable.WeaponDataTable'"), NULL, LOAD_None, NULL);
	PlayerData = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/New/Character/PlayerStatus.PlayerStatus'"), NULL, LOAD_None, NULL);
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