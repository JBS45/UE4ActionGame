// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewKatana.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"
#include "Particles/ParticleSystemComponent.h"

AMyNewKatana::AMyNewKatana() {

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

	KatanaParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	KatanaParticle->SetupAttachment(MainMesh);


	AttackUpRate = 0.15f;
	AttackDownRate = 0.15f;

}
void AMyNewKatana::BeginPlay() {
	Super::BeginPlay();
}
void AMyNewKatana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickDamageRate(DeltaTime);
}

void AMyNewKatana::SetEnable(bool IsOn) {
	Super::SetEnable(IsOn);
	SubMesh2->SetVisibility(IsEnable);
	if (IsEnable == false) {
		AttackRateReset();
	}
}
void AMyNewKatana::HitResult(float AttackRate) {
	if (IsEnable) {
		FVector StartLocation = MainMesh->GetSocketLocation("Hand");
		FVector EndLocation = MainMesh->GetSocketLocation("TrailStart");
		bool result = HitCheck(StartLocation, EndLocation, AttackRate);
		if (result) {
			AttackSuccess();
			GlowOn();
		}
	}
}

void AMyNewKatana::TickDamageRate(float delta) {
	if (IsEnable) {
		if (DamageRate > 1.0f) {
			DamageRate -= AttackDownRate * delta;
			KatanaParticle->InstanceParameters[0].Scalar -= 150 * delta;
		}
		else {
			DamageRate = 1.0f;
			KatanaParticle->InstanceParameters[0].Scalar = 0;
		}
		SubMesh2->SetScalarParameterValueOnMaterials("Num", 20 * (DamageRate-1));
	}
}
void AMyNewKatana::AttackSuccess() {
	if (DamageRate < 3.0f) {
		DamageRate += AttackUpRate;
		KatanaParticle->InstanceParameters[0].Scalar += 50;
	}
	else {
		DamageRate = 3.0f;
		KatanaParticle->InstanceParameters[0].Scalar = 1000;
	}
}

void AMyNewKatana::InitWeapon(const FNewWeaponData& data, AMyNewCharacter* owner) {
	Super::InitWeapon(data, owner);

	SubMesh2->SetMaterial(0, EnergyMaterial);

	if (Particle != nullptr) {
		KatanaParticle->Template = Particle;
		FParticleSysParam Param;
		Param.Name = "Spawn";
		Param.ParamType = EParticleSysParamType::PSPT_Scalar;
		Param.Scalar = 0;
		KatanaParticle->InstanceParameters.Add(Param);
	}
}
void AMyNewKatana::AttackRateReset() {
	DamageRate = 1.0f;
	KatanaParticle->InstanceParameters[0].Scalar = 0;
}