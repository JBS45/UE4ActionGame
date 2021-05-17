// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "MyTestProject.h"
#include "BaseEnum.h"
#include "BaseStatus.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UParticleSystemComponent;
class UAudioComponent;
class USoundCue;
class USoundEffectComponent;

UCLASS()
class MYTESTPROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
		float WeaponSize;

	UPROPERTY(VisibleAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* TrailParticle1;
	UPROPERTY(VisibleAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* TrailParticle2;
	UPROPERTY(VisibleAnywhere, Category = "Trail", meta = (AllowPrivateAccess = "true"))
		float TrailValue = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundEffectComponent* SwingAudio;
	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundEffectComponent* HitAudio;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FBaseStatus Status;

	bool CanCollision;

	TArray<class ABaseMonster*> DamagedMonster;

	class ACharacter* WeaponOwner;
	float DamageRate;
public:
	void SetUpWeapon(EWeaponType type, USkeletalMesh* mesh, ACharacter* owner);
	void SetEnableWeapon(bool value);
	void HitCheck();
	void ResetDamagedMonster();
	void SetOverlapEvent(bool IsOn);
	void SetDamageRate(float value);

	void SetAttackMode(bool IsOn);
	void TrailOn();
	void TrailOff();
	void TrailSet(UParticleSystem* template1, UParticleSystem* template2);

	void PlaySwingAudio(USoundCue * sooundcue);
private:
	FVector CurrentBoxSize;
	FVector CurrentBoxLocation;
};
