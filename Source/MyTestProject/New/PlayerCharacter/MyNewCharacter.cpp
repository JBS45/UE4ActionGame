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
#include "../PlayerCharacterComponents/MyNewInputBuffer.h"
#include "../../UI/BaseWidget.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"


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


	MainAudio = CreateDefaultSubobject<UAudioComponent>("MainAudio");
	MainAudio->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	MainAudio->bAutoActivate = false;


	SubAudio = CreateDefaultSubobject<UAudioComponent>("SubAudio");
	SubAudio->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	SubAudio->bAutoActivate = false;

	/*Mesh */
	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/New/Character/Animation/NewCharacterAnimBP.NewCharacterAnimBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}

	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel6);
	GetMesh()->SetCollisionProfileName("PlayerHitBox");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;


	RightTrail = CreateDefaultSubobject<UParticleSystemComponent>("AnimTrail1");
	RightTrail->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Hand_R");
	RightTrail->bAutoActivate = false;
	LeftTrail = CreateDefaultSubobject<UParticleSystemComponent>("AnimTrail2");
	LeftTrail->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Hand_L");
	LeftTrail->bAutoActivate = false;

	BuffParticle = CreateDefaultSubobject<UParticleSystemComponent>("BuffParticle");
	BuffParticle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	BuffParticle->bAutoActivate = false;

	PlayingMontage = false;
	IsEvadeFlag = false;

}

void AMyNewCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	AnimInst = Cast<UMyNewCharacterAnimInstance>(GetMesh()->GetAnimInstance());
}
// Called when the game starts or when spawned
void AMyNewCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInst->SetInputBuffer(Cast<IResetInputBuffer>(PlayerController->GetInputBuffer()));

	WeaponManager->SetInit(this);
	PlayerController->ChangeWeaponType.AddUObject(WeaponManager, &UMyNewWeaponManager::ChangeWeaponState);

	StatusManager->InitStatus(this);
	StatusManager->Attach(Cast<IUpdateStatus>(PlayerController->GetPlayerHUD()));
	StatusManager->PlayerDeadDel.BindUObject(PlayerController, &AMyNewPlayerController::PlayerDead);
	StatusManager->StaminaExhaustionDel.AddUObject(PlayerController, &AMyNewPlayerController::StaminaExhuastion);


	AttackDel.BindUObject(PlayerController->GetPlayerHUD(), &UBaseWidget::UseDamageText);

	PlayerController->ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);
	PlayerController->ChangeActionState.Broadcast(ENewActionState::E_NONE);
	PlayerController->ChangeWeaponType.Broadcast(ENewWeaponType::E_DUAL);
}

// Called every frame
void AMyNewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BuffTimer(DeltaTime);

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
			StatusManager->SubStaminaFlag(ENewStaminaState::E_IDLE);
			break;
		case ENewPlayerState::E_SPRINT:
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			StatusManager->AddStaminaFlag(ENewStaminaState::E_SPRINT);
			break;
		case ENewPlayerState::E_BATTLE:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			StatusManager->SubStaminaFlag(ENewStaminaState::E_IDLE);
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
	case ENewActionState::E_EVADE:
		IsEvadeFlag = true;
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
bool AMyNewCharacter::IsPlayerAlive() const {
	if (CurrentPlayerState == ENewPlayerState::E_DEAD)
		return false;
	else
		return true;
}
void AMyNewCharacter::PlaySound(USoundCue* cue) {
	MainAudio->SetSound(cue);
	MainAudio->Play();
}

void AMyNewCharacter::ApplyAttack(const FHitResult& Hit, float damage, float condDamage, float critical)
{
	auto Receiver = Cast<INewDamageInterface>(Hit.GetActor());
	bool IsCritical = false;
	bool IsWeak = false;
	int32 FinalDamage = 0;
	float CalculratedDamage = 0.0f;
	float CalculratedCondDamage = 0.0f;

	int32 CriticalChance = FMath::RandRange(0, 99);

	if (CriticalChance <= critical) {
		IsCritical = true;
		CalculratedDamage = 1.5f * damage;
		CalculratedCondDamage = 1.5f *condDamage;
	}
	else {
		CalculratedDamage = damage;
		CalculratedCondDamage = condDamage;
	}

	if ((Receiver != nullptr) && Receiver->IsAlive()) {
		FinalDamage = Receiver->TakeAttack(Hit, CalculratedDamage, CalculratedCondDamage, this, IsWeak, ENewMonsterDamageType::E_NORMAL,NULL);
	}

	if (IsCritical) {
		//크리티컬용 이펙트
	}
	else {
		//일반 데미지
	}

	if (FinalDamage <= 0) return;

	AttackDel.ExecuteIfBound(Hit.ImpactPoint, FinalDamage, IsWeak);
}
int32 AMyNewCharacter::TakeAttack(const FHitResult& Hit, float damage, float condDamage, class ACharacter* damageCauser, bool& IsWeak, ENewMonsterDamageType type, float knockback)
{
	FVector KnockBackDirection = -Hit.ImpactNormal;

	if (IsEvadeFlag) {
		TSharedPtr<PlayerBuff> Evade = TSharedPtr<PlayerBuff>(new EvadeBuff(ENewBuffType::E_EVADE, 1.0f));
		TSharedPtr<PlayerBuff> Time = TSharedPtr<PlayerBuff>(new TimeDilation(ENewBuffType::E_TIMEDELAY, 0.2f));
		Evade->BeginBuff(*this);
		Time->BeginBuff(*this);
		Buff.Add(Evade);
		Buff.Add(Time);
		IsEvadeFlag = false;
	}

	switch (type)
	{
	case ENewMonsterDamageType::E_NORMAL:
		//노말로 피격 방향 판단
		//애니메이션 재생
		break;
	case ENewMonsterDamageType::E_KNOCKBACK:
		//넉백
		//런쳐 캐릭터
		break;
	case ENewMonsterDamageType::E_ROAR:
		//블러
		//로어 피격
		break;
	default:
		break;
	}

	StatusManager->TakeDamage(damage);

	return 0;
}
bool AMyNewCharacter::IsAlive() {
	if (CurrentPlayerState != ENewPlayerState::E_DEAD) {
		return true;
	}
	return false;
}

void AMyNewCharacter::BuffTimer(float delta) {
	if (Buff.Num() > 0) {
		for (int i = 0; i < Buff.Num();) {
			if (Buff[i]->GetIsValid()) {
				Buff[i]->TickBuff(delta);
				i++;
			}
			else {
				Buff.RemoveAt(i);
			}
		}
	}
}
void AMyNewCharacter::DualBuff() {
	if (Buff.ContainsByPredicate([](const TSharedPtr<PlayerBuff>& Rhs) { return Rhs->GetType() == ENewBuffType::E_DUALDAMAGE; })) {
		auto DualBuff = Buff.FindByPredicate([](const TSharedPtr<PlayerBuff>& Rhs) { return Rhs->GetType() == ENewBuffType::E_DUALDAMAGE; });
		DualBuff->Get()->EndBuff();
	}
	else {
		TSharedPtr<PlayerBuff> DualBuff = TSharedPtr<PlayerBuff>(new DamageBuff(ENewBuffType::E_DUALDAMAGE, 1.0f));
		DualBuff->BeginBuff(*this);
		Buff.Add(DualBuff);
	}
}
void AMyNewCharacter::SetMaterailMesh(bool IsVisible) {
	MaterialMesh->SetVisibility(IsVisible);
	MaterialMesh->SetHiddenInGame(!IsVisible);
}

void AMyNewCharacter::SetParticlce(bool IsOn) {
	if (IsOn == true) {
		RightTrail->Activate();
		RightTrail->BeginTrails("Hand_R", "lowerarm_r", ETrailWidthMode::ETrailWidthMode_FromFirst, 1.0f);
		LeftTrail->Activate();
		LeftTrail->BeginTrails("Hand_L", "lowerarm_l", ETrailWidthMode::ETrailWidthMode_FromFirst, 1.0f);
		BuffParticle->Activate();
	}
	else {
		BuffParticle->Deactivate();
		RightTrail->EndTrails();
		LeftTrail->EndTrails();
	}
}