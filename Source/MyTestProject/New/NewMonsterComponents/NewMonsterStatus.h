// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerCharacterComponents/StatusComponent.h"
#include "NewMonsterStruct.h"
#include "NewMonsterStatus.generated.h"

DECLARE_DELEGATE(FMonsterDead);

class AMyNewBaseMonster;

struct MonsterPartsDataInGame {
public:
	ENewMonsterPartsType Type;
	EMonsterConditionType ConditionType;
	
	int32 AccruedDamage;
	int32 AccruedConditionDamage;

	bool CanDestory;
	bool IsBreak;
	bool IsWeak;
	float DestroyLimit;
	float CauseConditionLimit;
	float Defence;

	TArray<FName> Bones;
	const MonsterPartsDataInGame& Init (const FMonsterPartsData& rhs);
	void SetCauseConditionLimit(float value);
};

UCLASS()
class MYTESTPROJECT_API UNewMonsterStatus : public UStatusComponent
{
	GENERATED_BODY()
	
public:
	UNewMonsterStatus();
	
	void SetStatusData(float damage, float hp, UDataTable* partsdata);
protected:
	AMyNewBaseMonster* Owner;
	TMap<FName, TSharedPtr<MonsterPartsDataInGame>> HitBox;
	TMap<ENewMonsterPartsType, TSharedPtr<MonsterPartsDataInGame>> HitBoxPart;

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float Damage;
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float MaxHp;
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float CurrentHp;

protected:
	void InitHitBox(UDataTable* partsData);
	void SearchChildBone(USkeletalMeshComponent* mesh, FName CurrentBone,const TArray<FName>& BonesInBody, TSharedPtr<MonsterPartsDataInGame>& HitBoxData, TArray<FName> &SearchArray);
	bool CheckBreakPart(MonsterPartsDataInGame& target);
	bool CheckConditionPart(MonsterPartsDataInGame& target);
public:
	FMonsterDead MonsterDeadDel;
public:
	virtual int32 TakeDamage(const FHitResult& hit, float damage, float condDamage, bool& IsWeak);
	void SetBroken(ENewMonsterPartsType type);
	FORCEINLINE void GetHitBoxData(ENewMonsterPartsType type, TArray<MonsterPartsDataInGame*>& out) {out.Emplace(HitBoxPart[type].Get());}
	FORCEINLINE float GetDamage() { return Damage; }
};
