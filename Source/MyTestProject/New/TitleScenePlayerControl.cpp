// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleScenePlayerControl.h"
#include "Camera/CameraActor.h"
#include "../UI/TitleScreen.h"
#include "Kismet/GameplayStatics.h"

void ATitleScenePlayerControl::BeginPlay() {
	Super::BeginPlay();

	ViewTarget = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));

	if (ViewTarget != nullptr) {
		SetViewTarget(ViewTarget);
	}
	auto Widget = CreateWidget<UTitleScreen>(this,TitleScreen);
	Widget->AddToViewport();
	bShowMouseCursor = true;
}