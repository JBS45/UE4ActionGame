// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "HitCollisionManager.h"
#include "BossCollisionManager.generated.h"


/**
 *
 */
UCLASS()
class MYTESTPROJECT_API UBossCollisionManager : public UHitCollisionManager
{
	GENERATED_BODY()
public:
	UBossCollisionManager();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//new
	virtual void InitHitBox(UDataTable* data, USkeletalMeshComponent* mesh) override;
	virtual bool ReceiveDamage(const FHitResult& hit, const FName socketName, float dmg, int32& outDamage, bool& weak) override;
};
