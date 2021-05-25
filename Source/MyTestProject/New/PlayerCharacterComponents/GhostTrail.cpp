// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostTrail.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PoseableMeshComponent.h"



AGhostTrail::AGhostTrail() {
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UPoseableMeshComponent>("Mesh");
}
void AGhostTrail::Tick(float DeltaTime) {
	if (IsTrailOn) {
		TrailCheck(DeltaTime);
	}
}
void AGhostTrail::InitTrail(USkeletalMesh* mesh, UMaterialInstance* material) {
	Mesh->SetSkeletalMesh(mesh);
	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	int materialCount = Mesh->GetMaterials().Num();
	for (int i = 0; i < materialCount; ++i) {
		Mesh->SetMaterial(i, material);
	}
	IsTrailOn = false;
}
void AGhostTrail::TrailOn(USkeletalMeshComponent* copyMesh, float remainTime) {
	IsTrailOn = true;
	RemainTime = remainTime;
	Mesh->CopyPoseFromSkeletalComponent(copyMesh);
	Mesh->SetWorldTransform(copyMesh->GetComponentTransform());
	Mesh->SetVisibility(true);
}
void AGhostTrail::TrailCheck(float delta) {
	RemainTime -= delta;
	if (RemainTime <= 0) {
		IsTrailOn = false;
		Mesh->SetVisibility(false);
	}
}
