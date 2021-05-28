// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematiceTriggerBox.h"
#include "Components/BoxComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "PlayerCharacter/MyNewCharacter.h"
#include "PlayerCharacter/MyNewPlayerController.h"
#include "NewMonster/MyNewBossMonster.h"
#include "NewMonster/MyNewBaseMonster.h"
#include "NewMonster/NewMonsterSpawner.h"
#include "MovieSceneSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BGMManager.h"


// Called when the game starts or when spawned
ACinematiceTriggerBox::ACinematiceTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetCollisionProfileName("TriggerBox");
	IsPlayCinematic = false;
}

// Called when the game starts or when spawned
void ACinematiceTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACinematiceTriggerBox::OnOverlapBegin);
}

// Called every frame
void ACinematiceTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACinematiceTriggerBox::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (!IsPlayCinematic) {
		Player = Cast<AMyNewCharacter>(OtherActor);
		if (IsValid(Player)) {
			PlaySequence();
		}
	}
}

void ACinematiceTriggerBox::PlaySequence() {
	if (BossCinematic == nullptr) return;
	FMovieSceneSequencePlaybackSettings setting;
	ALevelSequenceActor* Out;
	ULevelSequencePlayer* SequncePlay = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BossCinematic, setting, Out);
	auto Tag = BossCinematic->FindBindingByTag("Boss");
	AMyNewBaseMonster* Boss = Cast<AMyNewBaseMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyNewBossMonster::StaticClass()));
	Boss->SetActorLocationAndRotation(Boss->GetMonsterArea()->GetActorLocation(), Boss->GetMonsterArea()->GetActorRotation() + FRotator(0, -90, 0));
	ABGMManager* BGM = Cast<ABGMManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABGMManager::StaticClass()));
	BGM->ControlVolume(0.0f);
	BossCinematic->UnbindPossessableObjects(Tag.GetGuid());
	BossCinematic->BindPossessableObject(Tag.GetGuid(), *Boss, GetWorld());
	SequncePlay->Play();
	IsPlayCinematic = true;
	SequncePlay->OnStop.AddDynamic(this, &ACinematiceTriggerBox::EndSequence);
	Player->GetPlayerController()->HideWidget(false);
	Player->DisableInput(Player->GetPlayerController());

}
void ACinematiceTriggerBox::EndSequence() {
	Player->GetPlayerController()->HideWidget(true);
	Player->EnableInput(Player->GetPlayerController());
	AMyNewBaseMonster* Boss = Cast<AMyNewBaseMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), AMyNewBossMonster::StaticClass()));
	Boss->SetActorLocationAndRotation(Boss->GetMonsterArea()->GetActorLocation(), Boss->GetMonsterArea()->GetActorRotation() + FRotator(0, -90, 0));
	ABGMManager* BGM = Cast<ABGMManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABGMManager::StaticClass()));
	BGM->ControlVolume(1.0f);
}