// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DestructibleComponent.h"


// Sets default values
ANewMonsterProjectile::ANewMonsterProjectile()
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
	HitBox->SetSphereRadius(150.0f);

	RootComponent = HitBox;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->SetUpdatedComponent(HitBox);
	Movement->InitialSpeed = ProjectileSpeed;
	Movement->MaxSpeed = ProjectileSpeed;
	Movement->bRotationFollowsVelocity = true;
	Movement->ProjectileGravityScale = 0.0f;
	Movement->SetAutoActivate(false);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(HitBox, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetRelativeLocation(FVector::ZeroVector);

	ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>("Explosion");
	RockDustParticle = CreateDefaultSubobject<UParticleSystemComponent>("RockDust");

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ROCKMESH(TEXT("StaticMesh'/Game/ParagonRampage/FX/Meshes/Debris/SM_Rock_02.SM_Rock_02'"));
	if (ROCKMESH.Succeeded()) {
		Mesh->SetStaticMesh(ROCKMESH.Object);
	}

	Mesh->SetRelativeScale3D(FVector::OneVector*2.0f);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>EXPLOSION_EFFECT(TEXT("ParticleSystem'/Game/ParagonRampage/FX/Particles/Abilities/RipNToss/FX/P_Rampage_Rock_HitWorld.P_Rampage_Rock_HitWorld'"));
	if (EXPLOSION_EFFECT.Succeeded()) {
		ExplosionParticle->Template = EXPLOSION_EFFECT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem>DUST_EFFECT(TEXT("ParticleSystem'/Game/ParagonRampage/FX/Particles/Abilities/RipNToss/FX/P_Rampage_Rock_Flying.P_Rampage_Rock_Flying'"));
	if (DUST_EFFECT.Succeeded()) {
		RockDustParticle->Template = DUST_EFFECT.Object;
	}


	ExplosionParticle->SetupAttachment(HitBox);
	ExplosionParticle->SetWorldScale3D(FVector::OneVector * ExplosionScale);
	ExplosionParticle->bAutoActivate = false;
	

	RockDustParticle->SetupAttachment(Mesh);;
	ExplosionParticle->SetWorldScale3D(FVector::OneVector * DustScale);


	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));

	LifeTime = 3.0f;
	IsShoot = false;


}

// Called when the game starts or when spawned
void ANewMonsterProjectile::BeginPlay()
{
	Super::BeginPlay();
	ExplosionParticle->SetWorldScale3D(FVector::OneVector * ExplosionScale);
	ExplosionParticle->SetWorldScale3D(FVector::OneVector * DustScale);
}

void ANewMonsterProjectile::Tick(float DeltaTime) {
	if (IsShoot) {
		FRotator Rotation = Mesh->GetRelativeRotation() + FRotator(0, 0, 180)*DeltaTime;
		Mesh->SetRelativeRotation(Rotation);
	}
}


void ANewMonsterProjectile::SpawnRock(float damage, float explosion, AActor* causer, ENewMonsterDamageType type, float force)
{
	Damage = damage;
	ProjectileCauser = causer;
	ExplosionRange = explosion;
	ProjectileDamageType = type;
	ProjectileForce = force;

	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Movement->SetUpdatedComponent(HitBox);
}
void ANewMonsterProjectile::ThrowRock(const FVector& ShootDirection)
{
	IsShoot = true;
	Movement->Velocity = ShootDirection * Movement->InitialSpeed;
	Movement->Activate(true);
	HitBox->OnComponentHit.AddDynamic(this, &ANewMonsterProjectile::OnHit);
	SetLifeSpan(15.0f);
}

void ANewMonsterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor == ProjectileCauser) return;

	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Explode(Hit);
	IsShoot = false;

}
void ANewMonsterProjectile::Explode(const FHitResult& Hit) {

	TArray<FHitResult> DetectResult;
	auto ExplodePoint = Hit.ImpactPoint;

	TArray<AActor*> Ignore;
	Ignore.Add(this);
	Ignore.Add(ProjectileCauser);

	bool Result = UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), ExplodePoint, ExplodePoint,
		ExplosionRange, TEXT("MonsterDamage"), true, Ignore, EDrawDebugTrace::ForOneFrame, DetectResult, true, FLinearColor::Blue, FLinearColor::Green, 1.0f);

	FVector HitLocation;
	FVector HitDir;

	Audio->Play();

	Mesh->SetVisibility(false);
	ExplosionParticle->SetWorldLocation(ExplodePoint);
	ExplosionParticle->Activate();
	ExplosionParticle->ActivateSystem();
	RockDustParticle->DeactivateSystem();

	bool IsWeak = false;
	int32 OutFinalDamage = 0;

	TArray<AActor*> repeatedActor;

	for (auto DetectCharacter : DetectResult) {

		auto Character = Cast<INewDamageInterface>(DetectCharacter.Actor);

		if (Character != nullptr && !repeatedActor.Contains(DetectCharacter.GetActor())) {
			Character->TakeAttack(Hit,Damage,NULL,nullptr,IsWeak, ProjectileDamageType, ProjectileForce);
			repeatedActor.Add(DetectCharacter.GetActor());
		}
	}

	SetLifeSpan(LifeTime);
}
