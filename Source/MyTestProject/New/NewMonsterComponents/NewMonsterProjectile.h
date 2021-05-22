// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewMonsterStruct.h"
#include "NewMonsterProjectile.generated.h"

class UAudioComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;

UCLASS()
class MYTESTPROJECT_API ANewMonsterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewMonsterProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* HitBox;
	UPROPERTY(VisibleAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		float HitBoxRange;

	UPROPERTY(VisibleAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* Audio;

	UPROPERTY(VisibleAnywhere, Category = Explosion, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* ExplosionParticle;
	UPROPERTY(VisibleAnywhere, Category = Explosion, meta = (AllowPrivateAccess = "true"))
		float ExplosionScale = 1.0f;
	UPROPERTY(VisibleAnywhere, Category = Dust, meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* RockDustParticle;
	UPROPERTY(VisibleAnywhere, Category = Dust, meta = (AllowPrivateAccess = "true"))
		float DustScale = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = Damage, meta = (AllowPrivateAccess = "true"))
		float DamageRate;
	UPROPERTY(VisibleAnywhere, Category = Damage, meta = (AllowPrivateAccess = "true"))
		float ExplosionRange;

	UPROPERTY(VisibleAnywhere, Category = Location, meta = (AllowPrivateAccess = "true"))
		FName LeftHand;
	UPROPERTY(VisibleAnywhere, Category = Location, meta = (AllowPrivateAccess = "true"))
		FName RightHand;

	UPROPERTY(VisibleAnywhere, Category = Time, meta = (AllowPrivateAccess = "true"))
		float LifeTime;


	UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* Movement;
	UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float ProjectileSpeed;

	bool IsShoot = false;

	float Damage = 0.0f;
	float ProjectileForce;
	AActor* ProjectileCauser;
	ENewMonsterDamageType ProjectileDamageType;


public:
	FORCEINLINE bool GetIsWorking() const;
	void ThrowRock(const FVector& ShootDirection);
	void SpawnRock(float damage, float explosion, AActor* causer, ENewMonsterDamageType type = ENewMonsterDamageType::E_KNOCKBACK, float force = 0);
private:
	void EnableMode();
	void Explode(const FHitResult& Hit);
private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
