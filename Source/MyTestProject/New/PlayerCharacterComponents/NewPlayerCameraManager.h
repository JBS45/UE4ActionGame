// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "NewPlayerCameraManager.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AMyNewCharacter;
class AMyNewBaseMonster;
class AMyNewPlayerController;

UCLASS()
class MYTESTPROJECT_API ANewPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
public:
	ANewPlayerCameraManager();
private:
	AMyNewCharacter* CurrentPlayer;

	UCameraComponent* MainCamera;
	USpringArmComponent* MainArm;
	UNewCameraDetect* Detector;

	UNewCameraShake* CameraShakeClass;
	AMyNewPlayerController* Controller;

	bool IsLockOn;
	int32 TargetIndex;

	float LockOnPitchMax;
public:
	void SetCamera(UCameraComponent* camera, USpringArmComponent* Arm, AMyNewCharacter* Player, UNewCameraDetect* Detect, AMyNewPlayerController* control);
	void ShakeCamera(float scale =1.0f);

	void ToggleLockOn();
	void ChangeLockOn();

	AMyNewBaseMonster* CameraLockOn(float delta);
private:
	bool NextTarget(int32& index);
};

UCLASS()
class MYTESTPROJECT_API UNewCameraShake : public UCameraShake
{
	GENERATED_BODY()

public:
	UNewCameraShake();
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UNewCameraDetect : public UActorComponent
{
	GENERATED_BODY()

public:
	UNewCameraDetect();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere, Category = "DetectRange", meta = (AllowPrivateAccess = "true"))
		USphereComponent* Range;
	UPROPERTY(EditAnywhere, Category = "Target", meta = (AllowPrivateAccess = "true"))
		TArray<AMyNewBaseMonster*> Targets;
public:
	void Init(float range, USkeletalMeshComponent* Mesh);
public:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	FORCEINLINE TArray<AMyNewBaseMonster*>& GetTargets() { return Targets; }
	FORCEINLINE float GetDetectRange() { return Range->GetScaledSphereRadius(); }
private:
	void CheckTarget();
};