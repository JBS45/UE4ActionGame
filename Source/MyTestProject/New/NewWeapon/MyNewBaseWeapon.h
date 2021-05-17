// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "MyNewBaseWeapon.generated.h"




class USkeletalMeshComponent;
class UAudioComponent;
class USoundCue;
class AMyNewCharacter;
class UMaterialInstance;
class UCapsuleComponent;

UCLASS()
class MYTESTPROJECT_API AMyNewBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyNewBaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MainMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SubMesh;


	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FVector AttachPutUpVector;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FRotator AttachPutUpRotator;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FVector AttachDrawVector;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FRotator AttachDrawRotator;

	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		UAudioComponent* MainAudio;
	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		UAudioComponent* SubAudio;
	UPROPERTY(EditAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundCue* SwingSoundEffect;

	UPROPERTY(EditAnywhere, Category = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* OutlineMaterial;


	TArray<class ABaseMonster*> DamagedMonster;

	ENewWeaponType WeaponType;
	AMyNewCharacter* WeaponOwner;

protected:
	UPROPERTY(EditAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		bool IsEnable;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float Damage;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float ConditionDamage;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float DamageRate;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float CriticlaRate;
	UPROPERTY(EditAnywhere, Category = "Size", meta = (AllowPrivateAccess = "true"))
		float Size;
public:
	virtual void SetEnable(bool IsOn);
	virtual void HitResult(EWeaponHand hand);
	virtual void PlaySwingAudio(EWeaponHand hand);
	virtual void InitWeapon(USkeletalMesh* mesh, float damage, float condDamage, float critical, float size);

	void SetUpWeapon(ENewWeaponType type, AMyNewCharacter* owner);
	void SetDamageRate(float value);
	void ResetDamagedMonster();

	void GlowOn();
	void GlowOff();
	void AttachDraw();
	void AttachPutUp();
	bool HitCheck(FVector start, FVector end);

	FORCEINLINE virtual float GetDamage();
	FORCEINLINE float GetConditionDamage();
	FORCEINLINE float GetCritical();
};