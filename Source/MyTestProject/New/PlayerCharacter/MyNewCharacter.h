// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "../PlayerCharacterInterface/NewUnrealInterface.h"
#include "../PlayerCharacterComponents/PlayerBuff.h"
#include "MyNewCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USkeletalMeshComponent;
class UMyNewCharacterAnimInstance;
class AMyNewPlayerController;
class AMyNewBaseWeapon;
class UChildActorComponent;
class UMyNewWeaponManager;
class UCharacterStatusComponent;
class UAudioComponent;
class USoundCue;

DECLARE_DELEGATE_ThreeParams(FAttackDel, FVector, int32, bool);


UCLASS()
class MYTESTPROJECT_API AMyNewCharacter : public ACharacter,public INewDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNewCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

private:
	UPROPERTY(VisibleAnywhere, Category = "Controller", meta = (AllowPrivateAccess = "true"))
		AMyNewPlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		UMyNewWeaponManager* WeaponManager;

	UPROPERTY(VisibleAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		UCharacterStatusComponent* StatusManager;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* BaseCamera;
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* BaseSpring;
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* SubCamera;
	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SubSpring;

	UPROPERTY(VisibleAnywhere, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* MaterialMesh;

	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewPlayerState CurrentPlayerState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewActionState CurrentActionState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewWeaponType CurrentWeaponType;

	UPROPERTY(EditAnywhere, Category = "MoveSpeed", meta = (AllowPrivateAccess = "true"))
		float RunSpeed;
	UPROPERTY(EditAnywhere, Category = "MoveSpeed", meta = (AllowPrivateAccess = "true"))
		float WalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = "AnimInst", meta = (AllowPrivateAccess = "true"))
		UMyNewCharacterAnimInstance* AnimInst;

	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		UAudioComponent* MainAudio;
	UPROPERTY(VisibleAnywhere, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		UAudioComponent* SubAudio;

	UPROPERTY(EditAnywhere, Category = "Buff", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* RightTrail;
	UPROPERTY(EditAnywhere, Category = "Buff", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* LeftTrail;
	UPROPERTY(EditAnywhere, Category = "Buff", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* BuffParticle;


		TArray<TSharedPtr<PlayerBuff>> Buff;

	bool PlayingMontage;
	bool IsEvadeFlag;

public:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void CameraLookUp(float NewAxisValue);
	void CameraTurn(float NewAxisValue);

	UCameraComponent* GetBaseCamera();
	UCameraComponent* GetSubCamera();

	void ChangePlayerState(const ENewPlayerState state);
	void ChangeActionState(const ENewActionState state);
	void ChangeWeaponState(const ENewWeaponType weapon);
	void PlaySound(class USoundCue* cue);
	void SetMaterailMesh(bool IsVisible);
	void SetParticlce(bool IsOn);
	void DualBuff();
public:
	FAttackDel AttackDel;

	virtual void ApplyAttack(const FHitResult& Hit, float damage, float condDamage, float critical) override;
	virtual int32 TakeAttack(const FHitResult& Hit, float damage, float condDamage, class ACharacter* damageCauser, bool& IsWeak, ENewMonsterDamageType type, float knockback) override;
	virtual bool IsAlive() override;
public:
	FORCEINLINE bool IsPlayerAlive() const;
	FORCEINLINE AMyNewPlayerController* GetPlayerController() const { return PlayerController; };
	FORCEINLINE void SetPlayerController(AMyNewPlayerController* control) { PlayerController = control; };
	FORCEINLINE ENewPlayerState GetCurrentPlayerState() const { return CurrentPlayerState; };
	FORCEINLINE ENewActionState GetCurrentActionState() const { return CurrentActionState; };
	FORCEINLINE ENewWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; };
	FORCEINLINE UMyNewCharacterAnimInstance* GetAnimInst() const { return AnimInst; };
	FORCEINLINE USkeletalMeshComponent* GetMainMesh() const { return GetMesh(); };
	FORCEINLINE UMyNewWeaponManager* GetWeaponManager() const { return WeaponManager; };
	FORCEINLINE UCharacterStatusComponent* GetStatusManager() const { return StatusManager; };
	FORCEINLINE void ResetEvadeFlag() { IsEvadeFlag = false; }
private:
	void BuffTimer(float delta);

};
