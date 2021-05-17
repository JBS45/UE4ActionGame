// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewBaseWeapon.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"

// Sets default values
AMyNewBaseWeapon::AMyNewBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<UCapsuleComponent>("RottCapsule");
	RootComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = RootComp;

	MainMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	MainMesh->SetupAttachment(RootComp);
	MainAudio = CreateDefaultSubobject<UAudioComponent>("MainAudio");
	MainAudio->AttachToComponent(MainMesh, FAttachmentTransformRules::KeepRelativeTransform, "TrailStart");
	MainAudio->SetSound(SwingSoundEffect);

	SubMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaterailMesh"));
	SubMesh->SetupAttachment(MainMesh);
	SubMesh->SetVisibility(false);
	SubMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SubMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	SubMesh->SetMasterPoseComponent(MainMesh);


	SubAudio = CreateDefaultSubobject<UAudioComponent>("SubAudio");
	SubAudio->AttachToComponent(MainMesh, FAttachmentTransformRules::KeepRelativeTransform, "TrailStart");
	SubAudio->SetSound(SwingSoundEffect);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> OUTLINEMATERIAL(TEXT("MaterialInstanceConstant'/Game/New/Material/OutLineMaterial_Inst.OutLineMaterial_Inst'"));

	if (OUTLINEMATERIAL.Succeeded()) {
		OutlineMaterial = OUTLINEMATERIAL.Object;
	}

	DamageRate = 1.0f;
	Damage = 30.0f;
	ConditionDamage = 60.0f;
	CriticlaRate = 20.0f;
	Size = 40.0f;

	AttachDrawVector = FVector::ZeroVector;
	AttachDrawRotator = FRotator(20, 0, 0);

	AttachPutUpVector = FVector::ZeroVector;
	AttachPutUpRotator = FRotator(0, 0, 0);
}

// Called when the game starts or when spawned
void AMyNewBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	SubMesh->SetVisibility(false);
}

// Called every frame
void AMyNewBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyNewBaseWeapon::SetEnable(bool IsOn) {
	IsEnable = IsOn;
	SubMesh->SetVisibility(false);
}
void AMyNewBaseWeapon::SetUpWeapon(ENewWeaponType type, AMyNewCharacter* owner) {
	WeaponOwner = owner;
	WeaponType = type;
	AttachPutUp();
}
void AMyNewBaseWeapon::SetDamageRate(float value) {
	DamageRate = value;
}
void AMyNewBaseWeapon::ResetDamagedMonster() {
	DamagedMonster.Empty();
	GlowOff();
}

void AMyNewBaseWeapon::PlaySwingAudio(EWeaponHand hand) {
	if (IsEnable) {
		MainAudio->Play();
		SubAudio->Play();
	}
}
void AMyNewBaseWeapon::AttachDraw() {
	SetActorRelativeLocation(AttachDrawVector);
	SetActorRelativeRotation(AttachDrawRotator);
}
void AMyNewBaseWeapon::AttachPutUp() {
	SetActorRelativeLocation(AttachPutUpVector);
	SetActorRelativeRotation(AttachPutUpRotator);
}

float AMyNewBaseWeapon::GetDamage() {
	return Damage;
}
float AMyNewBaseWeapon::GetConditionDamage() {
	return ConditionDamage;
}
float AMyNewBaseWeapon::GetCritical() {
	return CriticlaRate;
}

void AMyNewBaseWeapon::HitResult(EWeaponHand hand) {
	if (IsEnable) {
		FVector StartLocation = MainMesh->GetSocketLocation("Hand");
		FVector EndLocation = MainMesh->GetSocketLocation("TrailStart");

		if (HitCheck(StartLocation, EndLocation)) {
			GlowOn();
		}
	}
}

bool AMyNewBaseWeapon::HitCheck(FVector start, FVector end) {

	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);


	FRotator Rotation = FRotator::ZeroRotator;


	TArray<AActor*> Ignore;
	Ignore.Add(this);
	Ignore.Add(WeaponOwner);

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, Size, "PlayerDamage", true, Ignore, EDrawDebugTrace::ForOneFrame, DetectResult, true, FLinearColor::Green, FLinearColor::Black, 0.5f);
	//DrawDebugBox(GetWorld(), Location, CurrentBoxSize, Rotation.Quaternion(), FColor::Blue, false, 1.0f);

	/*for (auto result : DetectResult) {
		//auto DamageActor = Cast<IDamageInterface>(result.Actor);
		//if (DamageActor != nullptr && !DamagedMonster.Contains(result.Actor)) {
			if (IsValid(WeaponOwner)) {
				//Cast<IDamageInterface>(WeaponOwner)->ApplyDamageFunc(result, DamageRate, EDamageType::E_NORMAL, NULL);
			}
			DamagedMonster.Add(Cast<ABaseMonster>(result.Actor));
		}//
	}*/
	if (DamagedMonster.Num() <= 0) {
		return false;
	}
	else {

		return true;
	}

}

void AMyNewBaseWeapon::GlowOn() {
	SubMesh->SetVisibility(true);
}
void AMyNewBaseWeapon::GlowOff() {
	SubMesh->SetVisibility(false);
}
void AMyNewBaseWeapon::InitWeapon(USkeletalMesh* mesh, float damage, float condDamage, float critical, float size) {
	MainMesh->SetSkeletalMesh(mesh);
	SubMesh->SetSkeletalMesh(mesh);
	Damage = damage;
	ConditionDamage = condDamage;
	CriticlaRate = critical;
	Size = size;

	SubMesh->SetMaterial(0, OutlineMaterial);
	SubMesh->SetVisibility(false);
}