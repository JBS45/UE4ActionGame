// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCollisionManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Monster/BaseMonster.h"
#include "Animation/Skeleton.h"

UBossCollisionManager::UBossCollisionManager() {

}
void UBossCollisionManager::BeginPlay()
{
	Super::BeginPlay();
}

void UBossCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBossCollisionManager::InitHitBox(UDataTable* data, USkeletalMeshComponent* mesh)
{
	DataTable = data;
	auto Names = DataTable->GetRowNames();

	auto Physics = mesh->SkeletalMesh->PhysicsAsset;
	auto body = Physics->SkeletalBodySetups;

	TArray<FName> Parts;

	for (auto element : body) {
		Parts.Add(element->BoneName);
	}


	for (auto name : Names) {
		auto row = DataTable->FindRow<FMonsterParts>(name, FString(TEXT("Init Hit box Parts data")));

		SearchChild(mesh, row->PartRootBoneName, row->Part);

		auto CollisionPart = NewObject<UHitCollisionPart>();
		CollisionPart->SetUpData(*row);

		for (auto Label : PartLabel) {
			if (Parts.Contains(Label.Key) && !HitBox.Contains(Label.Key)) {
				HitBox.Add(Label.Key, CollisionPart);
				HitBoxLabel.Add(row->Part, CollisionPart);
			}
		}
	}

}
bool UBossCollisionManager::ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg, int32& outDamage, bool& weak) {
	FName BoneName = socketName;
	if (!HitBox.Contains(BoneName)) return false;

	auto PartStatus = HitBox[BoneName];

	if (PartStatus->CheckGetDamageThisPart(dmg, outDamage, weak)) {
		Cast<ABaseMonster>(hit.Actor)->SetBrokenState(PartStatus->GetPart());
		return true;
	}
	return false;
}