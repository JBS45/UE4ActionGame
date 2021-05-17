// Fill out your copyright notice in the Description page of Project Settings.

#include "HitCollisionManager.h"
#include "../Monster/BaseMonster.h"
#include "Animation/Skeleton.h"

// Sets default values for this component's properties
UHitCollisionManager::UHitCollisionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHitCollisionManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UHitCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHitCollisionManager::InitHitBox(UDataTable* data, USkeletalMeshComponent* mesh)
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
bool UHitCollisionManager::ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg, int32& outDamage, bool& weak) {
	FName BoneName = socketName;
	if (!HitBox.Contains(BoneName)) return false;

	auto PartStatus = HitBox[BoneName];

	if (PartStatus->CheckGetDamageThisPart(dmg, outDamage, weak)) {
		Cast<ABaseMonster>(hit.Actor)->SetBrokenState(PartStatus->GetPart());
		return true;
	}
	return false;
}
void UHitCollisionManager::SearchChild(USkeletalMeshComponent* mesh, FName CurrentBone, EMonsterPartsType type) {
	USkeleton* Skeleton = mesh->SkeletalMesh->Skeleton;

	if (Skeleton == nullptr) return;

	int currentIndex = mesh->GetBoneIndex(CurrentBone);

	TArray <int32> Children;
	Skeleton->GetChildBones(currentIndex, Children);


	if (Children.Num() == 0) {
		PartLabel.Add(CurrentBone, type);
		return;
	}

	for (auto child : Children) {
		if (PartLabel.Contains(mesh->GetBoneName(child))) break;

		SearchChild(mesh, mesh->GetBoneName(child), type);
	}

	if (!PartLabel.Contains(CurrentBone))
	{
		PartLabel.Add(CurrentBone, type);
	}
}

void UHitCollisionManager::BrokenPart(EMonsterPartsType type) {
	HitBoxLabel[type]->SetBroken();
}

UHitCollisionPart::UHitCollisionPart() {
	CanDestroy = false;
	IsDestroy = false;
	IsWeak = false;
	TotalDamage = 0.0f;
	DestroyDamage = 0.0f;
	Defence = 0.0f;
}

void UHitCollisionPart::SetUpData(const FMonsterParts data) {
	Part = data.Part;
	CanDestroy = data.CanDestory;
	IsDestroy = false;
	IsWeak = data.IsWeak;
	TotalDamage = 0.0f;
	DestroyDamage = data.DestroyDamage;
	Defence = data.Defence;
}

bool UHitCollisionPart::CheckGetDamageThisPart(float dmg, int32& OutDamage, bool& weak) {

	int32 AcculateDamage = FMath::RoundToInt(((100 - Defence) / 100)*dmg);
	TotalDamage += AcculateDamage;
	OutDamage = AcculateDamage;
	weak = IsWeak;

	if (CanDestroy &&
		!IsDestroy &&
		(TotalDamage >= DestroyDamage)) {

		return true;
	}

	return false;
}
void UHitCollisionPart::SetBroken() {
	CanDestroy = false;
	IsDestroy = true;
	Defence = FMath::CeilToFloat(Defence / 2);
}
EMonsterPartsType UHitCollisionPart::GetPart() {
	return Part;
}