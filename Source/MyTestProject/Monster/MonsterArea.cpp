// Fill out your copyright notice in the Description page of Project Settings.



#include "MonsterArea.h"
#include "../BaseGameMode.h"
#include "../BaseEnum.h"
#include "../Player/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "MonsterAIController.h"
#include "Components/SplineComponent.h"

// Sets default values
AMonsterArea::AMonsterArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsPlayerInRange = false;

	Area = CreateDefaultSubobject<USphereComponent>("AREA");
	Area->SetCollisionProfileName("DetectPlayer");

	RootComponent = Area;

	Spawner = CreateDefaultSubobject<AMonsterSpawner>(TEXT("SPAWNER"));

	Area->OnComponentBeginOverlap.AddDynamic(this, &AMonsterArea::OnOverlapBegin);
	Area->OnComponentEndOverlap.AddDynamic(this, &AMonsterArea::OnOverlapEnd);
	OnceSpawn = true;
}

// Called when the game starts or when spawned
void AMonsterArea::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnMonsterType != nullptr) {

		auto GameMode = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		auto MonsterID = SpawnMonsterType.GetDefaultObject()->GetMonsterID();
		SpawnMonsterStatus = GameMode->GetMonsterData(MonsterID);
	}
	MonsterRegenTimer = MonsterRegenCoolTime;

	Spawn();
}

// Called every frame
void AMonsterArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpawnUpdate(DeltaTime);

}
void AMonsterArea::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	auto target = Cast <ABaseCharacter>(OtherActor);
	TESTLOG(Warning, TEXT("InRange"));
	if (target != nullptr) {
		IsPlayerInRange = true;
	}
}
void AMonsterArea::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	auto target = Cast <ABaseCharacter>(OtherActor);
	TESTLOG(Warning, TEXT("OutRange"));
	if (target != nullptr) {
		IsPlayerInRange = false;
	}
}
void AMonsterArea::SpawnUpdate(float DeltaSeconds) {
	if (!IsPlayerInRange && MonsterRegenCoolTime >= 0) {
		CurrentMonsterCount = Monsters.Num();
		MonsterRegenTimer += DeltaSeconds;

		if (CurrentMonsterCount < MaxMonsterCount &&
			MonsterRegenTimer >= MonsterRegenCoolTime) {

			if (OnceSpawn) return;

			Spawn();
			MonsterRegenTimer = 0.0f;
		}
	}

}
void AMonsterArea::Spawn() {

	if (SpawnMonsterType != nullptr) {
		ABaseMonster* result;
		if (SpawnPoint == nullptr) {
			result = Spawner->SpawnMonster(SpawnMonsterType, GetActorLocation(), SpawnRange);
		}
		else {
			result = Spawner->SpawnMonster(SpawnMonsterType, SpawnPoint->GetActorLocation(), SpawnRange);
		}
		result->MonsterInit(SpawnMonsterStatus);
		result->SetActiveArea(this);
		//Cast<AMonsterAIController>(result->GetController())->SetUpPatrolPoints(PatrolPoint);
		Monsters.Add(result);
	}
	CurrentMonsterCount = Monsters.Num();
}
void AMonsterArea::Reset() {
	/*for (auto monster : Monsters) {
		monster->SetIsFoundPlayer(false);
	}*/
}
void AMonsterArea::Notify(ACharacter* target) {
	/*for (auto monster : Monsters) {
		monster->SetIsFoundPlayer(true);
		monster->SetTarget(target);
		monster->ChangeMonsterState(EMonsterStateType::E_BATTLE);
	}*/
}

bool AMonsterArea::GetIsPlayerInRange() {
	return IsPlayerInRange;
}
TArray<ATargetPoint*> AMonsterArea::GetPatrolPoint() {
	return PatrolPoint;
}
TArray<ABaseMonster*> AMonsterArea::GetMonsters() {
	return Monsters;
}
