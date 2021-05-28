// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewGameMode.h"
#include "MyNewGameState.h"
#include "PlayerCharacter/MyNewPlayerController.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "MyNewGameInstance.h"


AMyNewGameMode::AMyNewGameMode() {
	GameStateClass = AMyNewGameState::StaticClass();
	Record = 0.0f;
}
void AMyNewGameMode::PostLogin(APlayerController * NewPlayer) {
	auto GameInst = Cast<UMyNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInst->GetIsInit() == false) {
		GameInst->DataLoad();
	}
	Super::PostLogin(NewPlayer);
}
void AMyNewGameMode::BeginPlay() {
	auto GameInst = Cast<UMyNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInst->GetIsInit() == false) {
		GameInst->DataLoad();
	}
}
void AMyNewGameMode::Clear(float Time) {
	auto control = Cast<AMyNewPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	control->HideWidget(false);
	control->GameClearWidget(ClearUI);
	//Ŭ����
	//�÷��̾� ����
	//�÷��̾� ��Ʈ�ѷ��� ī�޶� �Ŵ��� ���� �� �ؽ���
	//HUD Ŭ����� ����
	Record = Time;
}

void AMyNewGameMode::Respawn(AMyNewPlayerController& Controller) {
	auto PlayerSpawnPoint = FindPlayerStart(&Controller, "PlayerSpawn");
	auto Character = SpawnDefaultPawnFor(&Controller, PlayerSpawnPoint);
	Controller.Possess(Character);
}