// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "../MyTestProject.h"
#include "BaseMonster.h"
#include "Boss.generated.h"

/**
 *
 */
class UDecalComponent;
class ADragonFireBall;
class USoundEffectComponent;
class ARockProjectile;
class UStunState;

UCLASS()
class MYTESTPROJECT_API ABoss : public ABaseMonster
{
	GENERATED_BODY()

public:
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void ChangeMonsterState(EMonsterState state) override;
	virtual void Roar() override;
	virtual void SetBrokenState(EMonsterPartsType brokenPart) override;

public:
	ARockProjectile* SpawnProjectile();
	void ShootProjectile(ARockProjectile* rock);
private:
	UPROPERTY(VisibleAnywhere, Category = "Material")
		USkeletalMeshComponent* MaterialMesh;

	UPROPERTY(VisibleAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* TrailLeftHand;
	UPROPERTY(VisibleAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* TrailRightHand;

	UPROPERTY(EditAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UParticleSystem> HitVFX;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsStun;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
		UStunState* StunState;
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ARockProjectile> ProjectileType;
public:
	FORCEINLINE bool GetIsStun();
	FORCEINLINE void SetIsStun(bool IsOn);
	FORCEINLINE UStunState* GetStunState();

};
