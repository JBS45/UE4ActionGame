// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "GameFramework/Actor.h"
#include "RockProjectile.generated.h"

class USoundEffectComponent;
class UDestructibleComponent;
class ABaseMonster;

UCLASS()
class MYTESTPROJECT_API ARockProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* HitBox;
	UPROPERTY(VisibleAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		float HitBoxRange;

	UPROPERTY(VisibleAnywhere, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		UDestructibleComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Sound")
		USoundEffectComponent* SoundEffectComp;

	UPROPERTY(VisibleAnywhere, Category = Explosion)
		class UParticleSystemComponent* ExplosionParticle;
	UPROPERTY(VisibleAnywhere, Category = Explosion)
		UParticleSystem* ExplosionTemplate;
	UPROPERTY(VisibleAnywhere, Category = Explosion, meta = (AllowPrivateAccess = "true"))
		float DamageRate;
	UPROPERTY(VisibleAnywhere, Category = Explosion, meta = (AllowPrivateAccess = "true"))
		float ExplosionRange;

	UPROPERTY(VisibleAnywhere, Category = Location, meta = (AllowPrivateAccess = "true"))
		FName LeftHand;
	UPROPERTY(VisibleAnywhere, Category = Location, meta = (AllowPrivateAccess = "true"))
		FName RightHand;

	UPROPERTY(VisibleAnywhere, Category = Time, meta = (AllowPrivateAccess = "true"))
		float LifeTime;


	UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* Movement;
	UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
		float ProjectileSpeed;

	bool IsShoot = false;

	float Damage = 0.0f;
	float ProjectileForce;
	AActor* ProjectileCauser;
	EDamageType ProjectileDamageType;


public:
	FORCEINLINE bool GetIsWorking() const;
	void ThrowRock(const FVector& ShootDirection);
	void SpawnRock(float damage, float explosion, AActor* causer, EDamageType type = EDamageType::E_KNOCKBACK, float force = 0);
private:
	void EnableMode();
	void Explode(const FHitResult& Hit);
private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
