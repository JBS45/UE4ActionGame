// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewKatana.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"

AMyNewDual::AMyNewDual() {


	SubMesh2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaterailMesh2"));
	SubMesh2->SetupAttachment(MainMesh);
	SubMesh2->SetVisibility(true);
	SubMesh2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SubMesh2->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	SubMesh2->SetMasterPoseComponent(MainMesh);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> ENERGYMATERIAL(TEXT("MaterialInstanceConstant'/Game/New/Material/SwordMaterial_Inst.SwordMaterial_Inst'"));

	if (ENERGYMATERIAL.Succeeded()) {
		EnergyMaterial = ENERGYMATERIAL.Object;
	}


	AttackUpRate = 0.05f;
	AttackDownRate = 0.15f;

}
void AMyNewDual::BeginPlay() {
	Super::BeginPlay();
}
void AMyNewDual::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickDamageRate(DeltaTime);
}

void AMyNewDual::SetEnable(bool IsOn) {
	Super::SetEnable(IsOn);
	SubMesh2->SetVisibility(IsEnable);
	if (IsEnable == false) {
		DamageRate = 1.0f;
	}
}
void AMyNewDual::PlaySwingAudio(EWeaponHand hand) {

}

void AMyNewDual::HitResult(EWeaponHand hand) {

	FVector StartLocation;
	FVector EndLocation;
	if (HitCheck(StartLocation, EndLocation)) {
		AttackSuccess();
		GlowOn();
	}
}

float AMyNewDual::GetDamage() {
	return Damage * DamageRate;
}

void AMyNewDual::TickDamageRate(float delta) {
	if (IsEnable) {
		if (DamageRate >= 1.0f) {
			DamageRate -= AttackDownRate * delta;
		}
		else {
			DamageRate = 1.0f;
		}
		SubMesh2->SetScalarParameterValueOnMaterials("Num", 20 * (1 - DamageRate));
	}
}
void AMyNewDual::AttackSuccess() {
	if (DamageRate <= 2.0f) {
		DamageRate += AttackUpRate;
	}
	else {
		DamageRate = 2.0f;
	}
}

void AMyNewDual::InitWeapon(USkeletalMesh* mesh, float damage, float condDamage, float critical, float size) {
	Super::InitWeapon(mesh, damage, condDamage, critical, size);

	SubMesh2->SetMaterial(0, EnergyMaterial);
}