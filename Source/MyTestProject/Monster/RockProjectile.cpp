// Fill out your copyright notice in the Description page of Project Settings.


#include "RockProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../Components/MyInterface.h"
#include "../Components/SoundEffectComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DestructibleComponent.h"


// Sets default values
ARockProjectile::ARockProjectile()
{

	PrimaryActorTick.bCanEverTick = true;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	HitBoxRange = 80.0f;
	DamageRate = 1.5f;
	ProjectileSpeed = 3000.0f;
	ExplosionRange = 200.0f;

	HitBox = CreateDefaultSubobject<USphereComponent>("HitBox");
	HitBox->InitSphereRadius(HitBoxRange);
	HitBox->SetCollisionProfileName("MonsterProjectile");
	RootComponent = HitBox;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->SetUpdatedComponent(HitBox);
	Movement->InitialSpeed = ProjectileSpeed;
	Movement->MaxSpeed = ProjectileSpeed;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 0.0f;
	Movement->SetAutoActivate(false);

	Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(HitBox, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetRelativeLocation(FVector::ZeroVector);

	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>("Explosion");
	static ConstructorHelpers::FObjectFinder<UParticleSystem>EXPLOSION_EFFECT(TEXT("ParticleSystem'/Game/ParagonRampage/FX/Particles/Abilities/RipNToss/FX/P_Rampage_Rock_HitCharacter.P_Rampage_Rock_HitCharacter'"));
	if (EXPLOSION_EFFECT.Succeeded()) {
		ExplosionTemplate = EXPLOSION_EFFECT.Object;
	}

	ExplosionParticle->SetupAttachment(HitBox);
	ExplosionParticle->Template = ExplosionTemplate;
	ExplosionParticle->SetWorldScale3D(FVector::OneVector * 2.0f);
	ExplosionParticle->bAutoActivate = false;

	SoundEffectComp = CreateDefaultSubobject<USoundEffectComponent>(TEXT("SOUNDEFFECTCOMP"));

	LifeTime = 8.0f;
	IsShoot = false;

}

// Called when the game starts or when spawned
void ARockProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ARockProjectile::Tick(float DeltaTime) {
	if (IsShoot) {
		FRotator Rotation = Mesh->GetRelativeRotation() + FRotator(0, 0, 180)*DeltaTime;
		Mesh->SetRelativeRotation(Rotation);
	}
}


void ARockProjectile::SpawnRock(float damage, float explosion, AActor* causer, EDamageType type, float force)
{
	Damage = damage;
	ProjectileCauser = causer;
	ExplosionRange = explosion;
	ProjectileDamageType = type;
	ProjectileForce = force;

	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Movement->SetUpdatedComponent(HitBox);
}
void ARockProjectile::ThrowRock(const FVector& ShootDirection)
{
	IsShoot = true;
	Movement->Velocity = ShootDirection * Movement->InitialSpeed;
	Movement->Activate(true);
	HitBox->OnComponentHit.AddDynamic(this, &ARockProjectile::OnHit);
}

void ARockProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor == ProjectileCauser) return;

	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	Mesh->ApplyRadiusDamage(1000.0f, Hit.ImpactPoint, 100.0f, 1000.0f, true);
	Explode(Hit);
	IsShoot = false;

}
void ARockProjectile::Explode(const FHitResult& Hit) {

	TArray<FHitResult> DetectResult;
	auto ExplodePoint = Hit.ImpactPoint;

	TArray<AActor*> Ignore;
	Ignore.Add(this);
	Ignore.Add(ProjectileCauser);

	bool Result = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), ExplodePoint, ExplodePoint,
		ExplosionRange, TEXT("MonsterDamage"), true, Ignore, EDrawDebugTrace::ForOneFrame, DetectResult, true, FLinearColor::Blue, FLinearColor::Green, 1.0f);

	FVector HitLocation;
	FVector HitDir;

	SoundEffectComp->PlaySE();

	ExplosionParticle->SetWorldLocation(ExplodePoint);
	ExplosionParticle->Activate();



	bool IsWeak = false;
	int32 OutFinalDamage = 0;

	TArray<AActor*> repeatedActor;

	for (auto DetectCharacter : DetectResult) {

		auto Character = Cast<IDamageInterface>(DetectCharacter.Actor);

		if (Character != nullptr && !repeatedActor.Contains(DetectCharacter.GetActor())) {
			TESTLOG(Warning, TEXT("%s"), *(DetectCharacter.Actor->GetName()));
			Character->TakeDamageFunc(IsWeak, OutFinalDamage, this, DetectCharacter, DamageRate*Damage, ProjectileDamageType, NULL);
			repeatedActor.Add(DetectCharacter.GetActor());
		}
	}

	SetLifeSpan(LifeTime);
}
