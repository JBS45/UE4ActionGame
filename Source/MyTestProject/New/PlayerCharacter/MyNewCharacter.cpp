// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyNewCharacterAnimInstance.h"
#include "MyNewPlayerController.h"
#include "../NewWeapon/MyNewBaseWeapon.h"
#include "Components/ChildActorComponent.h"
#include "../NewWeapon/MyNewWeaponManager.h"
#include "../PlayerCharacterComponents/CharacterStatusComponent.h"


// Sets default values
AMyNewCharacter::AMyNewCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Player")));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetCapsuleComponent()->bDynamicObstacle = true;

	WeaponManager = CreateDefaultSubobject<UMyNewWeaponManager>("WeaponManager");
	StatusManager = CreateDefaultSubobject<UCharacterStatusComponent>("StatusManager");

	BaseCamera = CreateDefaultSubobject<UCameraComponent>("BaseCamera");
	SubCamera = CreateDefaultSubobject<UCameraComponent>("SubCamera");
	BaseSpring = CreateDefaultSubobject<USpringArmComponent>("BaseSpring");
	SubSpring = CreateDefaultSubobject<USpringArmComponent>("SubSpring");

	BaseSpring->SetupAttachment(GetCapsuleComponent());
	BaseCamera->SetupAttachment(BaseSpring);

	SubSpring->SetupAttachment(GetCapsuleComponent());
	SubCamera->SetupAttachment(SubSpring);

	/*Mesh*/
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	BaseSpring->TargetArmLength = 450.0f;
	BaseSpring->bEnableCameraLag = true;
	BaseSpring->CameraLagSpeed = 10.0f;

	FVector BaseCameraVector = FVector(0.0f, 0.0f, 20.0f);
	FRotator BaseCameraRotator = FRotator(-15.0f, 0.0f, 0.0f);

	FVector SwitchingCameraVector = FVector(0.0f, 0.0f, -20.0f);
	FRotator SwitchingCameraRotator = FRotator(-10.0f, 180.0f, 0.0f);

	SubSpring->SetRelativeLocation(SwitchingCameraVector);
	SubSpring->SetRelativeRotation(SwitchingCameraRotator);
	SubCamera->SetActive(false);

	BaseSpring->SetRelativeLocation(BaseCameraVector);
	BaseSpring->SetRelativeRotation(BaseCameraRotator);
	BaseSpring->bUsePawnControlRotation = true;
	BaseCamera->bUsePawnControlRotation = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	RunSpeed = 900.0f;
	WalkSpeed = 600.0f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	/*Mesh */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MESH(TEXT("/Game/ODSMannequin/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (SK_MESH.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MESH.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	MaterialMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MaterailMesh"));
	MaterialMesh->SetupAttachment(GetMesh());
	MaterialMesh->SetSkeletalMesh(SK_MESH.Object);
	MaterialMesh->SetVisibility(false);
	MaterialMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MaterialMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat::Identity);
	MaterialMesh->SetMasterPoseComponent(GetMesh());
	MaterialMesh->SetHiddenInGame(true);

	/*Mesh */
	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/New/Character/Animation/NewCharacterAnimBP.NewCharacterAnimBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}

	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel6);
	GetMesh()->SetCollisionProfileName("PlayerHitBox");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	PlayingMontage = false;


}

// Called when the game starts or when spawned
void AMyNewCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInst = Cast<UMyNewCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInst->SetInputBuffer(Cast<IResetInputBuffer>(PlayerController->GetInputBuffer()));

	WeaponManager->SetInit(this);
	PlayerController->ChangeWeaponType.AddUObject(WeaponManager, &UMyNewWeaponManager::ChangeWeaponState);

	StatusManager->InitStatus(this);
	StatusManager->Attach(Cast<IUpdateStatus>(PlayerController->GetPlayerHUD()));
	StatusManager->PlayerDeadDel.BindUObject(PlayerController, &AMyNewPlayerController::PlayerDead);
	StatusManager->StaminaExhaustionDel.BindUObject(PlayerController, &AMyNewPlayerController::StaminaExhuastion);

	PlayerController->ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);
	PlayerController->ChangeActionState.Broadcast(ENewActionState::E_NONE);
	PlayerController->ChangeWeaponType.Broadcast(ENewWeaponType::E_DUAL);
}

// Called every frame
void AMyNewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMyNewCharacter::MoveForward(float NewAxisValue)
{
	float Dist = BaseCamera->GetForwardVector().Size();
	FVector DirXY = FVector(BaseCamera->GetForwardVector().X, BaseCamera->GetForwardVector().Y, 0.0f);
	DirXY.Normalize();
	DirXY *= Dist;
	AddMovementInput(DirXY, NewAxisValue);
}

void AMyNewCharacter::MoveRight(float NewAxisValue)
{
	float Dist = BaseCamera->GetRightVector().Size();
	FVector DirXY = FVector(BaseCamera->GetRightVector().X, BaseCamera->GetRightVector().Y, 0.0f);
	DirXY.Normalize();
	DirXY *= Dist;
	AddMovementInput(DirXY, NewAxisValue);
}

void AMyNewCharacter::CameraLookUp(float NewAxisValue) {
	AddControllerPitchInput(NewAxisValue);
}
void AMyNewCharacter::CameraTurn(float NewAxisValue) {
	AddControllerYawInput(NewAxisValue);
}

UCameraComponent* AMyNewCharacter::GetBaseCamera() {
	return BaseCamera;
}
UCameraComponent* AMyNewCharacter::GetSubCamera() {
	return SubCamera;
}

void AMyNewCharacter::ChangePlayerState(const ENewPlayerState state) {
	if (CurrentPlayerState == state) return;
	CurrentPlayerState = state;
	switch (CurrentPlayerState)
	{
		case ENewPlayerState::E_IDLE:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		case ENewPlayerState::E_SPRINT:
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			break;
		case ENewPlayerState::E_BATTLE:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		case ENewPlayerState::E_DEAD:
			GetCharacterMovement()->Deactivate();
			break;
	}

}
void AMyNewCharacter::ChangeActionState(const ENewActionState state) {
	if (CurrentActionState == state) return;
	CurrentActionState = state;
	switch (CurrentActionState)
	{
	case ENewActionState::E_NONE:
		break;
	case ENewActionState::E_ACTION:
		break;
	case ENewActionState::E_HIT:
		break;
	case ENewActionState::E_KNOCKBACK:
		break;
	case ENewActionState::E_DOWN:
		break;
	}
}
void AMyNewCharacter::ChangeWeaponState(const ENewWeaponType weapon) {
	if (CurrentWeaponType == weapon) return;
	CurrentWeaponType = weapon;
	switch (CurrentWeaponType) {
	case ENewWeaponType::E_DUAL:
		//CurrentWeapon->SetEnable(false);
		//CurrentWeapon = Weapons[ENewWeaponType::E_DUAL];
		//CurrentWeapon->SetEnable(true);
		break;
	case ENewWeaponType::E_KATANA:
		//CurrentWeapon->SetEnable(false);
		//CurrentWeapon = Weapons[ENewWeaponType::E_KATANA];
		//CurrentWeapon->SetEnable(true);
		break;
	case ENewWeaponType::E_AXE:
		//CurrentWeapon->SetEnable(false);
		//CurrentWeapon = Weapons[ENewWeaponType::E_AXE];
		//CurrentWeapon->SetEnable(true);
		break;
	}
}

ENewPlayerState AMyNewCharacter::GetCurrentPlayerState() const {
	return CurrentPlayerState;
}
ENewActionState AMyNewCharacter::GetCurrentActionState() const {
	return CurrentActionState;
}
ENewWeaponType AMyNewCharacter::GetCurrentWeaponType() const {
	return CurrentWeaponType;
}
UMyNewCharacterAnimInstance* AMyNewCharacter::GetAnimInst() const {
	return AnimInst;
}
AMyNewPlayerController* AMyNewCharacter::GetPlayerController() const {
	return PlayerController;
}
void AMyNewCharacter::SetPlayerController(AMyNewPlayerController* control) {
	PlayerController = control;
}
USkeletalMeshComponent* AMyNewCharacter::GetMainMesh() const {
	return GetMesh();
}
UMyNewWeaponManager* AMyNewCharacter::GetWeaponManager() const {
	return WeaponManager;
}
UCharacterStatusComponent* AMyNewCharacter::GetStatusManager() const {
	return StatusManager;
}
