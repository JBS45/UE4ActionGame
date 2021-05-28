// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "GhostTrail.generated.h"

class USkeletalMeshComponent;
class UPoseableMeshComponent;
class AMyNewCharacter;


UCLASS()
class MYTESTPROJECT_API AGhostTrail : public AActor
{
	GENERATED_BODY()
public:
	AGhostTrail();
public:
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere, Category = "Ghost", meta = (AllowPrivateAccess = "true"))
	UPoseableMeshComponent* Mesh;

	bool IsTrailOn;
	float RemainTime;

public:
	void InitTrail(USkeletalMesh* mesh, UMaterialInstance* material);
	void TrailOn(USkeletalMeshComponent* copyMesh, float remainTime);
	void TrailCheck(float delta);
};
