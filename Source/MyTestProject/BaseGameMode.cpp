// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Player/BaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Player/BasePlayerController.h"

ABaseGameMode::ABaseGameMode() {

	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();

}

void ABaseGameMode::PostLogin(APlayerController * NewPlayer)
{
	auto DataTable = LoadObject<UDataTable>(NULL, TEXT("DataTable'/Game/DataTable/Monster/MonsterStat.MonsterStat'"), NULL, LOAD_None, NULL);

	auto Names = DataTable->GetRowNames();
	for (auto name : Names) {
		auto row = DataTable->FindRow<FMonsterStatus>(name, FString(TEXT("Init Monster data")));
		MonsterData.Add(row->MonsterID, *row);
	}

	TESTLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	PlayerControl = Cast<ABasePlayerController>(NewPlayer);
	TESTLOG(Warning, TEXT("PostLogin End"));
}

void ABaseGameMode::BeginPlay() {
	Super::BeginPlay();

	TESTLOG(Warning, TEXT("Game mode begin play"));
}

FMonsterStatus ABaseGameMode::GetMonsterData(uint8 MonsterId) {
	return MonsterData[MonsterId];
}

