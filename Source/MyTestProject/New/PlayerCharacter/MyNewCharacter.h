// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
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

UCLASS()
class MYTESTPROJECT_API AMyNewCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyNewCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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


	bool PlayingMontage;

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

	FORCEINLINE AMyNewPlayerController* GetPlayerController() const;
	FORCEINLINE void SetPlayerController(AMyNewPlayerController* control);
	FORCEINLINE ENewPlayerState GetCurrentPlayerState() const;
	FORCEINLINE ENewActionState GetCurrentActionState() const;
	FORCEINLINE ENewWeaponType GetCurrentWeaponType() const;
	FORCEINLINE UMyNewCharacterAnimInstance* GetAnimInst() const;
	FORCEINLINE USkeletalMeshComponent* GetMainMesh() const;
	FORCEINLINE UMyNewWeaponManager* GetWeaponManager() const;
	FORCEINLINE UCharacterStatusComponent* GetStatusManager() const;
};
