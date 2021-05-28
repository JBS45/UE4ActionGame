// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostTrailManager.h"
#include "GhostTrail.h"

UGhostTrailManager::UGhostTrailManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

	PoolLimit = 20;
}

// Called when the game starts or when spawned
void UGhostTrailManager::BeginPlay()
{
	Super::BeginPlay();
}
void UGhostTrailManager::EndPlay(const EEndPlayReason::Type type) {
	Super::EndPlay(type);

	while (Trails.Num() > 0) {
		Trails[0]->Destroy();
		Trails.RemoveAt(0);
	}
}

// Called every frame
void UGhostTrailManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsTrailOn) {
		CheckTrail(DeltaTime);
	}

}
void UGhostTrailManager::InitTrail(int32 limit, USkeletalMeshComponent* Mesh) {
	PoolLimit = limit;
	OwnerMesh = Mesh;
	for (int i = 0; i < PoolLimit; ++i) {
		auto Trail = GetWorld()->SpawnActor<AGhostTrail>(AGhostTrail::StaticClass(), GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		Trail->InitTrail(Mesh->SkeletalMesh, MaterialOrigin);
		Trails.Add(Trail);
	}
}

void UGhostTrailManager::MakeTrail(float runTime, float remainTime) {
	Interval = (runTime / PoolLimit);
	IntervalTimer = 0;
	IsTrailOn = true;
	RemainTime = remainTime;
	Trails[0]->TrailOn(OwnerMesh, RemainTime);

	RunTrailCount = 1;
}

void UGhostTrailManager::CheckTrail(float delta) {
	IntervalTimer += delta;
	if (IntervalTimer >= Interval && RunTrailCount < PoolLimit) {
		IntervalTimer -= Interval;
		Trails[RunTrailCount]->TrailOn(OwnerMesh, RemainTime);
		RunTrailCount++;
	}
	if (RunTrailCount >= PoolLimit) {
		IsTrailOn = false;
	}
}