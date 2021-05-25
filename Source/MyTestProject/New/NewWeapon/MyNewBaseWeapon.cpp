// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewBaseWeapon.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"
#include "../NewMonster/MyNewBaseMonster.h"
#include "Particles/ParticleSystemComponent.h"
#include "../PlayerCharacterComponents/CharacterStatusComponent.h"

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
	MainAudio->bAutoActivate = false;

	SubMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaterailMesh"));
	SubMesh->SetupAttachment(MainMesh);
	SubMesh->SetVisibility(false);
	SubMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SubMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	SubMesh->SetMasterPoseComponent(MainMesh);


	SubAudio = CreateDefaultSubobject<UAudioComponent>("SubAudio");
	SubAudio->AttachToComponent(MainMesh, FAttachmentTransformRules::KeepRelativeTransform, "TrailStart");
	SubAudio->SetSound(SwingSoundEffect);
	SubAudio->bAutoActivate = false;

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
	AttachDrawRotator = FRotator(0, 0, 0);

	AttachPutUpVector = FVector::ZeroVector;
	AttachPutUpRotator = FRotator(0, 0, 0);

	Trail1 = CreateDefaultSubobject<UParticleSystemComponent>("AnimTrail1");
	Trail1->AttachToComponent(MainMesh, FAttachmentTransformRules::KeepRelativeTransform, "BladeEnd");
	Trail1->bAutoActivate = false;
	Trail2 = CreateDefaultSubobject<UParticleSystemComponent>("AnimTrail2");
	Trail2->AttachToComponent(MainMesh, FAttachmentTransformRules::KeepRelativeTransform, "BladeEnd");
	Trail2->bAutoActivate = false;
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

void AMyNewBaseWeapon::SetDamageRate(float value) {
	DamageRate = value;
}
void AMyNewBaseWeapon::ResetDamagedMonster() {
	DamagedMonster.Empty();
	GlowOff();
}

void AMyNewBaseWeapon::BeginTrail() {
	if (Trail1->Template != nullptr/*&&Trail2->Template != nullptr*/) {
		Trail1->BeginTrails(FName("BladeEnd"), FName("BladeStart"), ETrailWidthMode::ETrailWidthMode_FromCentre, 1.0f);
		//Trail2->BeginTrails(FName("BladeEnd"), FName("BladeStart"), ETrailWidthMode::ETrailWidthMode_FromCentre, 1.0f);
	}
}
void AMyNewBaseWeapon::EndTrail() {
	Trail1->EndTrails();
	//Trail2->EndTrails();
}

void AMyNewBaseWeapon::PlaySwingAudio() {
	if (IsEnable) {
		MainAudio->Play();
	}
}
void AMyNewBaseWeapon::PlaySwingAudio(USoundCue* cue) {
	if (IsEnable) {
		SubAudio->SetSound(cue);
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

void AMyNewBaseWeapon::HitResult(float AttackRate){
	if (IsEnable) {
		FVector StartLocation = MainMesh->GetSocketLocation("Hand");
		FVector EndLocation = MainMesh->GetSocketLocation("TrailStart");
		bool result = HitCheck(StartLocation, EndLocation, AttackRate);
		if (result) {
			GlowOn();
		}
	}
}

bool AMyNewBaseWeapon::HitCheck(FVector start, FVector end, float AttackRate) {

	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);


	FRotator Rotation = FRotator::ZeroRotator;


	TArray<AActor*> Ignore;
	Ignore.Add(this);
	Ignore.Add(WeaponOwner);

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, Size, "PlayerDamage", true, Ignore, EDrawDebugTrace::ForOneFrame, DetectResult, true, FLinearColor::Green, FLinearColor::Black, 0.5f);

	UCharacterStatusComponent* status = WeaponOwner->GetStatusManager();

	if (DetectResult.Num() <= 0) {
		return false;
	}
	else {
		for (auto result : DetectResult) {
			auto Monster = Cast<AMyNewBaseMonster>(result.Actor);
			if (IsValid(Monster)) {
				if (!DamagedMonster.Contains(result.Actor)) {
					Cast<INewDamageInterface>(WeaponOwner)->ApplyAttack(result, status->GetDamage()*AttackRate, status->GetCondDamage(), status->GetCritical());
					DamagedMonster.Add(Monster);
				}
			}
		}
		return true;
	}

}

void AMyNewBaseWeapon::GlowOn() {
	SubMesh->SetVisibility(true);
}
void AMyNewBaseWeapon::GlowOff() {
	SubMesh->SetVisibility(false);
}
void AMyNewBaseWeapon::InitWeapon(const FNewWeaponData& data, AMyNewCharacter* owner) {
	WeaponOwner = owner;
	WeaponType = data.Type;
	MainMesh->SetSkeletalMesh(data.WeaponMesh);
	SubMesh->SetSkeletalMesh(data.WeaponMesh);
	Damage = data.BaseDamage;
	ConditionDamage = data.ConditionDamage;
	CriticlaRate = data.CriticalRate;
	Size = data.TraceSize;

	SubMesh->SetMaterial(0, OutlineMaterial);
	SubMesh->SetVisibility(false);

	MainAudio->SetSound(data.SwingSound);
}

