// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewGameState.h"
#include "MyNewGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/MyNewPlayerController.h"

AMyNewGameState::AMyNewGameState() {
	TargetID = 1;
	TargetCount = 1;
	DeadCount = 0;
}

void AMyNewGameState::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	Timer += DeltaSeconds;
}

void AMyNewGameState::Notify(uint8 MonsterID) {
	if (KillMonsterCount.Contains(MonsterID)) {
		KillMonsterCount[MonsterID]++;
	}
	else {
		KillMonsterCount.Add(MonsterID, 1);
	}

	if (KillMonsterCount[MonsterID] >= TargetCount) {
		Clear();
	}
}
void AMyNewGameState::Clear(){
	Cast<AMyNewGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->Clear(Timer);
}
void AMyNewGameState::PlayerDead(AMyNewPlayerController& controller){
	DeadCount++;
}
void AMyNewGameState::Respawn(AMyNewPlayerController& controller) {
	Cast<AMyNewGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->Respawn(controller);
}