// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "GameFramework/Pawn.h"
#include "Components/ActorComponent.h"
#include "MyInterface.h"
#include "DetectComponent.generated.h"

class USphereComponent;
class ABaseMonster;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDetectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetDetectSphere();

	void LockOnCamera();
	void LockOff();
	void SwitchingCamera();
	FORCEINLINE ABaseMonster* GetLockTarget();
	FORCEINLINE void Attach(ICameraLockOnInterface* obserever);
private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckTargets();
	void CheckLockOnTarget();
	void Notify();
private:
	UPROPERTY(EditAnywhere, Category = "DetectRange", meta = (AllowPrivateAccess = "true"))
		USphereComponent* DetectSphere;
	UPROPERTY(EditAnywhere, Category = "DetectRange", meta = (AllowPrivateAccess = "true"))
		FName CollisonProfile;
	UPROPERTY(EditAnywhere, Category = "DetectRange", meta = (AllowPrivateAccess = "true"))
		float Range;

	TArray<ICameraLockOnInterface*> LockOnObservers;

	bool IsLockOn;
	UPROPERTY(EditAnywhere, Category = "DetectRange", meta = (AllowPrivateAccess = "true"))
		TArray<ABaseMonster*> Targets;
	ABaseMonster* LockOnTarget;

	int CurrentTargetsIndex;
};
