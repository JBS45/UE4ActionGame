// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterSpawner.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "Kismet/GameplayStatics.h"
#include "../MyNewGameInstance.h"
#include "MyNewBaseMonster.h"
#include "NewMonsterController.h"
#include "../PlayerCharacter/MyNewCharacter.h"

// Sets default values
ANewMonsterSpawner::ANewMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MonsterArea = CreateDefaultSubobject<USphereComponent>("MonsterArea");
	SetRootComponent(MonsterArea);

	MonsterArea->SetCollisionProfileName("DetectPlayer");
	MonsterArea->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	MonsterArea->bDynamicObstacle = false;

	IsPlayerInArea = false;

	SpawnCoolTime = 20.0f;
	AutoSpawnTimer = 0;
}

// Called when the game starts or when spawned
void ANewMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SetUpData();
	SpawnMonster();
	MonsterArea->OnComponentBeginOverlap.AddDynamic(this, &ANewMonsterSpawner::OnPlayerInRange);
	MonsterArea->OnComponentEndOverlap.AddDynamic(this, &ANewMonsterSpawner::OnPlayerOutRange);
}

// Called every frame
void ANewMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPlayerInArea) {
		CheckPlayerState();
		//CheckMonsterState();
	}
	else {
		if (IsOnlyOnceSpawn == false) {
			AutoSpawn(DeltaTime);
		}
	}
}
void ANewMonsterSpawner::SetUpData() {

	UMyNewGameInstance* MyGameInstance = Cast<UMyNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	for (int i = 0; i < MonsterID.Num(); ++i) {
		auto Data = &MyGameInstance->GetMonsterData(MonsterID[i]);
		MonsterData.Add(MonsterID[i], Data);
	}
}
void ANewMonsterSpawner::SpawnMonster() {
	int Min = 0;
	int Max = MonsterID.Num() - 1;

	while (CurrMonsterCountInArea<MaxMonsterCountInArea)
	{
		uint8 RandomIDIndex = FMath::RandRange(Min, Max);
		Spawn(*MonsterData[MonsterID[RandomIDIndex]]);
		CurrMonsterCountInArea = MonstersInArea.Num();
	}
}
void ANewMonsterSpawner::Spawn(const FNewMonsterData& data){
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) {
		return;
	}
	FNavLocation SpawnLocation;
	NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), MonsterArea->GetScaledSphereRadius()/3, SpawnLocation);


	SpawnLocation.Location += FVector(0, 0, 50);
	AMyNewBaseMonster* MonsterActor = GetWorld()->SpawnActor<AMyNewBaseMonster>(data.MonsterActor, SpawnLocation.Location, FRotator::ZeroRotator);

	if (MonsterActor == nullptr) return;

	MonsterActor->SetUpMonster(data,this);

	MonstersInArea.Add(MonsterActor);
}


void ANewMonsterSpawner::OnPlayerInRange(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AMyNewCharacter* OverlapCharacter = Cast<AMyNewCharacter>(OtherActor);
	if (OverlapCharacter != nullptr&&OverlapCharacter->IsAlive()) {
		Player = OverlapCharacter;
		IsPlayerInArea = true;
	}
}

void ANewMonsterSpawner::OnPlayerOutRange(class UPrimitiveComponent* OverlappedComp,class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (Player == OtherActor) {
		Player = nullptr;
		IsPlayerInArea = false;
		CheckMonsterState();
		for (int i = 0; i < MonstersInArea.Num();++i) {
			MonstersInArea[i]->GetMonsterController()->ChangeStateDel.Broadcast(ENewMonsterState::E_IDLE);
			MonstersInArea[i]->GetMonsterController()->ResetTarget();
		}
	}
}
void ANewMonsterSpawner::CheckMonsterState() {
	for (int i = 0; i < MonstersInArea.Num();) {
		if (IsValid(MonstersInArea[i])&&MonstersInArea[i]->IsAlive()) {
			i++;
		}
		else {
			MonstersInArea.RemoveAt(i);
			CurrMonsterCountInArea = MonstersInArea.Num();
		}
	}
}
void ANewMonsterSpawner::CheckPlayerState() {
	if (Player->IsAlive()==false) {
		Player = nullptr;
		IsPlayerInArea = false;
		for (auto Monster : MonstersInArea) {
			Monster->GetMonsterController()->ChangeStateDel.Broadcast(ENewMonsterState::E_IDLE);
			Monster->GetMonsterController()->ResetTarget();
		}
	}
}
FVector ANewMonsterSpawner::FindPatrolPoint() {
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) {
		return FVector::ZeroVector;
	}
	FNavLocation PatrolLocation;
	NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), MonsterArea->GetScaledSphereRadius(), PatrolLocation);

	return PatrolLocation.Location;
}

void ANewMonsterSpawner::AutoSpawn(float delta) {
	if (CurrMonsterCountInArea < MaxMonsterCountInArea) {
		AutoSpawnTimer += delta;
		if (AutoSpawnTimer > SpawnCoolTime) {
			AutoSpawnTimer -= SpawnCoolTime;

			SpawnMonster();
		}
	}
}