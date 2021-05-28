// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GhostTrailManager.generated.h"

class AGhostTrail;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UGhostTrailManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UGhostTrailManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type type) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "TrailLimit", meta = (AllowPrivateAccess = "true"))
		int32 PoolLimit;

	USkeletalMeshComponent* OwnerMesh;

	UPROPERTY(EditAnywhere, Category = "Material", meta = (AllowPrivateAccess = "true"))
		UMaterialInstance* MaterialOrigin;

	TArray<AGhostTrail*> Trails;

	float Interval;
	float IntervalTimer;
	float RemainTime;

	int32 RunTrailCount;

	bool IsTrailOn;
public:
	void InitTrail(int32 limit, USkeletalMeshComponent* Mesh);
	void MakeTrail(float RunTime, float remainTime);
private:
	void CheckTrail(float delta);
};
