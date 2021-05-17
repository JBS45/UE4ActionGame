// Fill out your copyright notice in the Description page of Project Settings.



#include "BaseWeapon.h"
#include "Monster/BaseMonster.h"
#include "Player/BaseCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Collision/HitCollisionManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Components/SoundEffectComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Collision = CreateDefaultSubobject <UBoxComponent>(TEXT("DamaageBox"));

	RootComponent = Mesh;
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionObjectType(ECC_GameTraceChannel7);
	Collision->SetCollisionProfileName("PlayerDamage");

	TrailParticle1 = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticle1");
	TrailParticle1->SetupAttachment(Mesh);

	TrailParticle2 = CreateDefaultSubobject<UParticleSystemComponent>("TrailParticle2");
	TrailParticle2->SetupAttachment(Mesh);


	SwingAudio = CreateDefaultSubobject< USoundEffectComponent>("Swing");
	//SwingAudio->SetupAttachment(Mesh);

	HitAudio = CreateDefaultSubobject< USoundEffectComponent>("Hit");
	//HitAudio->SetupAttachment(Mesh);
	CanCollision = false;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanCollision) {
		HitCheck();
	}
}


void ABaseWeapon::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//if I use do once function I can Implement Attack
	//Attack rate use Animation curve

	if (OtherActor->GetClass()->IsChildOf<ABaseMonster>()) {
		if (!DamagedMonster.Contains(OtherActor)) {
			HitCheck();
		}

	}
}

void ABaseWeapon::SetUpWeapon(EWeaponType type, USkeletalMesh* mesh, ACharacter* owner) {
	switch (type) {
	case EWeaponType::E_DUAL:
		CurrentBoxSize = FVector(10, 10, 40);
		CurrentBoxLocation = FVector(0, 0, 40);
		WeaponSize = 25;
		break;
	case EWeaponType::E_HAMMER:
		CurrentBoxSize = FVector(20, 32, 80);
		CurrentBoxLocation = FVector(0, 0, 40);
		break;
	}
	Collision->SetBoxExtent(CurrentBoxSize);
	Collision->SetRelativeLocation(CurrentBoxLocation);


	WeaponOwner = owner;
	Mesh->SetSkeletalMesh(mesh);
	SetEnableWeapon(true);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnOverlapBegin);

}
void ABaseWeapon::SetEnableWeapon(bool value) {

	Mesh->SetHiddenInGame(!value);
	if (value) {
		Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else {
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SetActorTickEnabled(value);
}
void ABaseWeapon::HitCheck() {
	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);

	FVector Location = Collision->GetComponentLocation();
	FRotator Rotation = Collision->GetComponentRotation();

	/*GetWorld()->SweepMultiByChannel(
		DetectResult,
		Location,
		Location,
		Rotation.Quaternion(),
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeBox(CurrentBoxSize),
		params);*/

	TArray<AActor*> Ignore;
	Ignore.Add(this);
	Ignore.Add(WeaponOwner);

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), Mesh->GetSocketLocation("TrailEnd"), Mesh->GetSocketLocation("TrailStart"), WeaponSize, "PlayerDamage", true, Ignore, EDrawDebugTrace::ForOneFrame, DetectResult, true, FLinearColor::Green, FLinearColor::Black, 0.5f);
	//DrawDebugBox(GetWorld(), Location, CurrentBoxSize, Rotation.Quaternion(), FColor::Blue, false, 1.0f);


	for (auto result : DetectResult) {
		auto DamageActor = Cast<IDamageInterface>(result.Actor);
		if(DamageActor!=nullptr && !DamagedMonster.Contains(result.Actor)){
			if (Cast<ABaseCharacter>(WeaponOwner)) {
				TESTLOG(Warning, TEXT("%f"), DamageRate);
				Cast<IDamageInterface>(WeaponOwner)->ApplyDamageFunc(result, DamageRate, EDamageType::E_NORMAL, NULL);
			}
			DamagedMonster.Add(Cast<ABaseMonster>(result.Actor));
		}
	}
}

void ABaseWeapon::ResetDamagedMonster() {
	DamagedMonster.Empty();
}
void ABaseWeapon::SetOverlapEvent(bool IsOn) {
	Collision->SetGenerateOverlapEvents(IsOn);
	ResetDamagedMonster();
}

void ABaseWeapon::TrailOn() {
	TrailParticle1->Activate(true);
	TrailParticle2->Activate(true);
	TrailParticle1->BeginTrails("TrailStart", "TrailCenter", ETrailWidthMode::ETrailWidthMode_FromCentre, TrailValue);
	TrailParticle2->BeginTrails("TrailStart", "TrailCenter", ETrailWidthMode::ETrailWidthMode_FromCentre, TrailValue);
}
void ABaseWeapon::TrailOff() {
	TrailParticle1->EndTrails();
	TrailParticle2->EndTrails();
	TrailParticle1->Complete();
	TrailParticle2->Complete();
}

void ABaseWeapon::TrailSet(UParticleSystem* template1, UParticleSystem* template2) {
	TrailParticle1->SetTemplate(template1);
	TrailParticle2->SetTemplate(template2);
}
void ABaseWeapon::PlaySwingAudio(USoundCue * sooundcue) {
	if (sooundcue == nullptr) return;
	SwingAudio->PlaySE();
}
void ABaseWeapon::SetDamageRate(float value) {
	DamageRate = value;
}

void ABaseWeapon::SetAttackMode(bool IsOn) {
	CanCollision = IsOn;
	if (CanCollision) {
		TrailOn();
	}
	else {
		TrailOff();
		ResetDamagedMonster();
	}

}
