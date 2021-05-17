// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "GameFramework/Character.h"
#include "../Components/DetectComponent.h"
#include "../Components/MyInterface.h"
#include "BaseMonster.generated.h"

class UHitCollisionManager;
class UMonsterStatusManager;
class UMonsterAnimInstance;
class AMonsterAIController;
class AMonsterArea;
class ABaseCharacter;
class USoundEffectComponent;
class UPawnSensingComponent;
struct FKTaperedCapsuleElem;
class UCapsuleComponent;
class UHitCollisionManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeePawnDelegate, APawn*, Pawn);

UCLASS()
class MYTESTPROJECT_API ABaseMonster : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual bool MonsterInit(FMonsterStatus status);
	virtual bool TakeDamageFromPlayer(const FHitResult& hit, const FName socketName, float dmg, int32& outDmage, bool& weak)  PURE_VIRTUAL(ABaseMonster::TakeDamage, return false;);
	virtual void ChangeMonsterState(EMonsterState state) PURE_VIRTUAL(ABaseMonster::ChangeMonsterState);
	virtual void Dead();
	virtual void PartBroken(EMonsterPartsType brokenPart) PURE_VIRTUAL(ABaseMonster::PartBroken);
	virtual void Roar() PURE_VIRTUAL(ABaseMonster::Roar);

public:
	virtual void SetBrokenState(EMonsterPartsType brokenPart);
	virtual void ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);
	virtual void TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);
	UFUNCTION()
		void OnSeePawn(APawn *OtherPawn);

public:
	FORCEINLINE float GetAttackRange();
	FORCEINLINE bool GetIsAlive();
	FORCEINLINE void SetActiveArea(class AMonsterArea* area);
	FORCEINLINE AMonsterArea* GetActiveArea();
	FORCEINLINE UHitCollisionManager* GetCollisionManager();
	FORCEINLINE FString GetMonsterName() const;
	FORCEINLINE uint8 GetMonsterID() const;
	FORCEINLINE TArray<APawn*> GetTargetList();
	FORCEINLINE EMonsterState GetMonsterState();
	FORCEINLINE int32 GetBrokenState() const;
	FORCEINLINE ABaseCharacter* GetTarget();
	FORCEINLINE AMonsterAIController* GetMonsterController();
	FORCEINLINE UMonsterAnimInstance* GetAnimInst();
	FORCEINLINE UPawnSensingComponent* GetPawnSensing();
	FORCEINLINE float GetRoarRange();
	FORCEINLINE bool GetIsDown();
	FORCEINLINE float GetDetectRange();
	FORCEINLINE float GetDamage() const;

	FORCEINLINE UAnimMontage* GetMontage(FString name) const;

	void SetMeleeDamage(float damageRate, float force, EDamageType type, EMonsterPartsType EnablePart);
	void HitCheck(EMonsterPartsType Part, float damageRate, float knockbackDist, EDamageType damagetype);

protected:
	void SetDamageBox(TArray<class UCapsuleComponent*> arr);
	void ResetDamagedPlayer();

protected:

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float BasicSpeed = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float RunSpeed = 200.0f;
	UPROPERTY(VisibleAnywhere, Category = "CollisionManager", meta = (AllowPrivateAccess = "true"))
		UHitCollisionManager* HitManager;
	UPROPERTY(VisibleAnywhere, Category = "Hit", meta = (AllowPrivateAccess = "true"))
		FName LeftHand;
	UPROPERTY(VisibleAnywhere, Category = "Hit", meta = (AllowPrivateAccess = "true"))
		FName RightHand;
	UPROPERTY(VisibleAnywhere, Category = "Hit", meta = (AllowPrivateAccess = "true"))
		float HitRadius;



	UPROPERTY(VisibleAnywhere, Category = "Detect", meta = (AllowPrivateAccess = "true"))
		UPawnSensingComponent* Detect;
	UPROPERTY(EditAnywhere, Category = "Detect", meta = (AllowPrivateAccess = "true"))
		float PlayerDetectDistance;

	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundEffectComponent* SoundEffectComp;

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		class UMonsterStatusManager* Status;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float AttackRange;
	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		float RoarRange;


	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		FString MonsterName;
	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		uint8 MonsterID;



	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		EMonsterState CurrentState;
	UPROPERTY(EditAnywhere, Category = "State", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EMonsterBrokenParts"))
		int32 BrokenState;


	UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UMonsterAnimInstance* AnimInst;

	UPROPERTY(VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
		AMonsterAIController* MonsterController;

	UPROPERTY(VisibleAnywhere, Category = "Area", meta = (AllowPrivateAccess = "true"))
		AMonsterArea* ActiveArea;
	UPROPERTY(VisibleAnywhere, Category = "Area", meta = (AllowPrivateAccess = "true"))
		ABaseCharacter* Target;

protected:
	bool IsAttack = false;
	int MaxPatrolNode;
	int CurrentPatrolNode;
	TArray<ABaseCharacter*> DamagedCharacter;

	EDamageType AttackDamageType = EDamageType::E_NORMAL;
	float DamageRate = 1.0f;
	float KnockBackForce = 0.0f;
	EMonsterPartsType AttackEnablePart = EMonsterPartsType::E_HEAD;

};
