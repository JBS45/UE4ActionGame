#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "BaseCharAnimInstance.h"
#include "../Components/CharacterStatusManager.h"
#include "GameFramework/Character.h"
#include "../Components/MyInterface.h"
#include "../EffectClass.h"
#include "BaseCharacter.generated.h"



class ABaseWeapon;
class UDetectComponent;
class UPostProcessComponent;
class UCharacterIKComponent;
class UCharacterBlockComponent;
class UPostProcessComponent;
class UDetectComponent;
class USoundEffectComponent;
class UEffectClass;


UCLASS()
class MYTESTPROJECT_API ABaseCharacter : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController * NewController) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ECharacterState CurrentState;

	UPROPERTY(VisibleAnywhere, Category = "Status")
		EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, Category = "Status")
		class ABaseWeapon* LeftHand;
	UPROPERTY(EditAnywhere, Category = "Status")
		class ABaseWeapon* RightHand;

	UPROPERTY(EditAnywhere, Category = "Movvement", meta = (AllowPrivateAccess = "true"))
		float BasicSpeed = 800.0f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float RunSpeed = 1600.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* SwitchingCamera;
	UPROPERTY(EditAnywhere, Category = "Camera")
		USpringArmComponent* SwitchSpring;

	class USkeletalMesh* TempMesh;
	UPROPERTY(VisibleAnywhere, Category = "Material")
		USkeletalMeshComponent* MaterialMesh;
	UPROPERTY(VisibleAnywhere, Category = "Status")
		UCharacterStatusManager* StatusManager;
	UPROPERTY(VisibleAnywhere, Category = "Animation")
		UBaseCharAnimInstance* AnimInst;
	UPROPERTY(VisibleAnywhere, Category = "Detect")
		UDetectComponent* LockOnDetect;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UCharacterIKComponent* IKComp;
	UPROPERTY(VisibleAnywhere, Category = "Block")
		UCharacterBlockComponent* BlockComp;
	UPROPERTY(VisibleAnywhere, Category = "PostProcess")
		UPostProcessComponent* PostProcessMat;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
		USoundEffectComponent* SoundEffectComp;
	UPROPERTY(VisibleAnywhere, Category = "Effect")
		TArray<UEffectClass*> Effects;

	bool IsAlive = true;
	bool IsSprint = false;
	bool IsEvade = false;
	bool IsSpecial = false;




	float DetectRange = 2000.0f;

	AActor* BlockTarget;


public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		float CameraSpeed = 10.0f;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		float CurrentSpringArmLength = 500.0f;


	float MinCameraDistance = 150;
	float MaxCameraDistance = 600;


	FVector BaseCameraVector;
	FRotator BaseCameraRotator;

	FPlayerAttackDel AttackDel;
	FTimerHandle CheckTimer;
public:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void MoveCameraMaxDist();
	void CameraLockUp(float NewAxisValue);
	void CameraTurn(float NewAxisValue);

	void Sprint(bool sprint);
public:
	FORCEINLINE UBaseCharAnimInstance* GetAnimInst();
	FORCEINLINE UDetectComponent* GetLockOnDetect();
	FORCEINLINE EWeaponType GetWeaponState();
	FORCEINLINE UCharacterStatusManager* GetCharacterStatus();
	FORCEINLINE UCharacterIKComponent* GetIK();
	FORCEINLINE bool GetIsAlive() const;
	FORCEINLINE class ABaseWeapon* GetLeftHand();
	FORCEINLINE class ABaseWeapon* GetRightHand();
	FORCEINLINE void SetEvade(bool IsOn);
	FORCEINLINE ECharacterState GetState() const;
	FORCEINLINE TArray<UEffectClass*>* GetEffects();
	FORCEINLINE void SetEffects(UEffectClass* effect);

	void SetIsSprint(bool value);
	void ChangeWeapon(EWeaponType type);
	void CharacterChangeState(ECharacterState state);

	void CameraLockOn(class ABaseMonster* target);
	float GetDetectRange() { return DetectRange; }
	void SetInitWeapon();
	void DrawWeapon();
	void PutUpWeapon();

	void CharacterOverlapOtherActor(float delta);
	FORCEINLINE void WeaponLeftOverlapOn();
	FORCEINLINE void WeaponLeftOverlapOff();
	FORCEINLINE void WeaponRightOverlapOn();
	FORCEINLINE void WeaponRightOverlapOff();

	bool CaculateCritical();
	void RadialBlurOn();
	void RadialBlurOff();
	void SetMaterialMesh(bool IsOn);

	virtual void ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);
	virtual void TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0);

private:
	FVector BlockCheck(AActor* Actor);
	void Blocking(float delta);
};
