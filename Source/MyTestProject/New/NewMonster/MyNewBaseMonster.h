// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "MyNewBaseMonster.generated.h"

class UNewMonsterStatus;
class ANewMonsterController;
class ANewMonsterSpawner;
class UNewMonsterAnimInstance;
class AMyNewCharacter;
class UPawnSensingComponent;
class ANewMonsterProjectile;

UCLASS()
class MYTESTPROJECT_API AMyNewBaseMonster : public ACharacter,public INewDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNewBaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetUpMonster(const FNewMonsterData& data, ANewMonsterSpawner* area);
protected:
	UPROPERTY(VisibleAnywhere, Category = "ID", meta = (AllowPrivateAccess = "true"))
		uint8 MonsterID;
	float MonsterSize = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Controller", meta = (AllowPrivateAccess = "true"))
		ANewMonsterController* MonsterController;
	UPROPERTY(VisibleAnywhere, Category = "Area", meta = (AllowPrivateAccess = "true"))
		ANewMonsterSpawner* MonsterArea;

	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewMonsterState CurrentMonsterState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		EMonsterConditionState CurrentConditionState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "ENewMonsterBrokenPart"))
		int32 BrokenState;

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		UNewMonsterStatus* StatusManager;
	UPROPERTY(VisibleAnywhere, Category = "Anim", meta = (AllowPrivateAccess = "true"))
		UNewMonsterAnimInstance* AnimInst;

	UPROPERTY(VisibleAnywhere, Category = "Sight", meta = (AllowPrivateAccess = "true"))
		UPawnSensingComponent* MonsterSight;
	UPROPERTY(EditAnywhere, Category = "Sight", meta = (AllowPrivateAccess = "true"))
		float MonsterSightLength = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float WalkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float RunSpeed = 800.0f;

	UPROPERTY(VisibleAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ANewMonsterProjectile> ProjectileType;

protected:
	TArray<AMyNewCharacter*> DamagedCharacters;
protected:
	virtual void ChangeMonsterState(const ENewMonsterState state);
	virtual void ChangeMonsterConditionState(const EMonsterConditionState state);
protected:
	UFUNCTION()
		void OnFindPlayer(APawn *OtherPawn);
public:
	virtual void SetCondition(EMonsterConditionState type, float totalTime);
	virtual void SetBrokenPart(ENewMonsterPartsType brokenpart);
	virtual void ApplyAttack(const FHitResult& Hit, float damage, float condDamage, float critical) override;
	virtual int32 TakeAttack(const FHitResult& Hit, float damage, float condDamage, class ACharacter* damageCauser, bool& IsWeak, ENewMonsterDamageType type, float knockback) override;
	virtual bool IsAlive() override;

	void AttackCheck(ENewMonsterAttackPart part,float damage,ENewMonsterDamageType type = ENewMonsterDamageType::E_NORMAL,float knockback = 0);
	void HitCheck(const FName BoneName,const FKSphylElem& collision, float damage, float knockback, ENewMonsterDamageType type);
	void ClearDamagedCharacter();
public:
	FORCEINLINE ENewMonsterState& GetCurrentMonsterState() { return CurrentMonsterState; }
	FORCEINLINE EMonsterConditionState& GetCurrentConditionState() { return CurrentConditionState; }
	FORCEINLINE ANewMonsterController* GetMonsterController() { return MonsterController; }
	FORCEINLINE ANewMonsterSpawner* GetMonsterArea() { return MonsterArea; }
	FORCEINLINE UNewMonsterAnimInstance& GetAnimInst() { return *AnimInst; }
	FORCEINLINE int32 GetBrokenState() { return BrokenState; }
};
