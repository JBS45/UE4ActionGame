// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyNewBaseWeapon.h"
#include "MyNewKatana.generated.h"


class UMaterialInstance;

UCLASS()
class MYTESTPROJECT_API AMyNewDual : public AMyNewBaseWeapon
{
	GENERATED_BODY()
	
public:
	AMyNewDual();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* SubMesh2;

	UPROPERTY(EditAnywhere, Category = "Buff", meta = (AllowPrivateAccess = "true"))
		float AttackUpRate;
	UPROPERTY(EditAnywhere, Category = "Buff", meta = (AllowPrivateAccess = "true"))
		float AttackDownRate;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FRotator AttachPutUpRotate;
	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		UAudioComponent* AudioSub;
	UPROPERTY(EditAnywhere, Category = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* EnergyMaterial;
public:
	virtual void SetEnable(bool IsOn) override;
	virtual void HitResult(EWeaponHand hand) override;
	virtual void PlaySwingAudio(EWeaponHand hand) override;
	virtual void InitWeapon(USkeletalMesh* mesh, float damage, float condDamage, float critical, float size) override;

	void TickDamageRate(float delta);
	void AttackSuccess();
public:
	FORCEINLINE virtual float GetDamage() override;
};
