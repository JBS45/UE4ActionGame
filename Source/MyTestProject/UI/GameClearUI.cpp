// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameClearUI::NativeConstruct(){
	Super::NativeConstruct();
	TitleName = "MainTitle";
	Button = Cast<UButton>(GetWidgetFromName("GoToTitle"));
	Button->OnClicked.AddDynamic(this, &UGameClearUI::GoToTitle);
}

void UGameClearUI::GoToTitle() {
	UGameplayStatics::OpenLevel(GetWorld(), TitleName);
}