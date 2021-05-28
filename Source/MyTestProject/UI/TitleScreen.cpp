// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleScreen.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTitleScreen::NativeConstruct() {
	Super::NativeConstruct();

	LevelName = "MainPlayLevel";

	StartBtn = Cast<UButton>(GetWidgetFromName("StartButton"));
	StartBtn->OnClicked.AddDynamic(this, &UTitleScreen::GoToMainLevel);
	EndBtn = Cast<UButton>(GetWidgetFromName("QuitButton"));
	EndBtn->OnClicked.AddDynamic(this, &UTitleScreen::QuitGame);
}
void UTitleScreen::GoToMainLevel() {
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
void UTitleScreen::QuitGame() {
	UKismetSystemLibrary::QuitGame(GetWorld(),GetOwningPlayer(),EQuitPreference::Quit,false);
}