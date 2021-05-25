// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewGameMode.h"
#include "MyNewGameState.h"
#include "PlayerCharacter/MyNewPlayerController.h"
#include "Camera/CameraActor.h"

AMyNewGameMode::AMyNewGameMode() {
	GameStateClass = AMyNewGameState::StaticClass();
	Record = 0.0f;
}

void AMyNewGameMode::Clear(float Time) {
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