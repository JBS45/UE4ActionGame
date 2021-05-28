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
#include "Kismet/KismetMathLibrary.h"
#include "../PlayerCharacterComponents/NewPlayerCameraManager.h"
#include "../PlayerCharacterComponents/NewCollisionManager.h"
#include "Components/PostProcessComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "../PlayerCharacterComponents/GhostTrail.h"
#include "../PlayerCharacterComponents/GhostTrailManager.h"
#include "../MyNewGameMode.h"
#include "../../UI/FadeWidget.h"
#include "../MyNewGameState.h"



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
	BaseSpring = CreateDefaultSubobject<USpringArmComponent>("BaseSpring");

	BaseSpring->SetupAttachment(GetCapsuleComponent());
	BaseCamera->SetupAttachment(BaseSpring);

	CameraDetect = CreateDefaultSubobject<UNewCameraDetect>("CameraDetect");
	CameraDetect->Init(3500.0f,GetMesh());

	/*Mesh*/
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	BaseSpring->TargetArmLength = 450.0f;
	BaseSpring->bEnableCameraLag = true;
	BaseSpring->CameraLagSpeed = 10.0f;

	FVector BaseCameraVector = FVector(0.0f, 0.0f, 20.0f);
	FRotator BaseCameraRotator = FRotator(-15.0f, 0.0f, 0.0f);

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


	/*Mesh */
	static ConstructorHelpers::FClassFinder<UAnimInstance>MESH_ANIM(TEXT("AnimBlueprint'/Game/New/Character/Animation/NewCharacterAnimBP.NewCharacterAnimBP_C'"));
	if (MESH_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(MESH_ANIM.Class);
	}

	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel6);
	GetMesh()->SetCollisionProfileName("PlayerHitBox");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	CollisionManager = CreateDefaultSubobject<UNewCollisionManager>("collision");

	RightTrail = CreateDefaultSubobject<UParticleSystemComponent>("AnimTrail1");
	RightTrail->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Hand_R");
	RightTrail->bAutoActivate = false;
	LeftTrail = CreateDefaultSubobject<UParticleSystemComponent>("AnimTrail2");
	LeftTrail->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "Hand_L");
	LeftTrail->bAutoActivate = false;

	PostProcessMat = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcessMat->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	GhostTrail = CreateDefaultSubobject<UGhostTrailManager>("GhostTrail");

	static ConstructorHelpers::FObjectFinder<UMaterialInstance>BLURMAT(TEXT("MaterialInstanceConstant'/Game/New/Material/RadialBlur_Inst.RadialBlur_Inst'"));
	if (BLURMAT.Succeeded()) {
		BlurMat = BLURMAT.Object;
	}

	BuffParticle = CreateDefaultSubobject<UParticleSystemComponent>("BuffParticle");
	BuffParticle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	BuffParticle->bAutoActivate = false;

	PlayingMontage = false;
	IsEvadeFlag = false;

	GroundFriction = GetCharacterMovement()->GroundFriction;

	GhostTrailRunTime = 0.5f;
	GhostTrailRemainTime = 1.0f;
}

void AMyNewCharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();
	AnimInst = Cast<UMyNewCharacterAnimInstance>(GetMesh()->GetAnimInstance());
}
void AMyNewCharacter::PossessedBy(AController * NewController) {
	Super::PossessedBy(NewController);

	AnimInst->Init();
	AnimInst->SetInputBuffer(Cast<IResetInputBuffer>(PlayerController->GetInputBuffer()));

	WeaponManager->SetInit(this);
	PlayerController->ChangeWeaponType.AddUObject(WeaponManager, &UMyNewWeaponManager::ChangeWeaponState);

	PlayerController->GetCameraManager()->SetCamera(BaseCamera, BaseSpring,this, CameraDetect, PlayerController);

	StatusManager->InitStatus(this);
	StatusManager->Attach(Cast<IUpdateStatus>(PlayerController->GetPlayerHUD()));
	StatusManager->PlayerDeadDel.BindUObject(PlayerController, &AMyNewPlayerController::PlayerDead);
	StatusManager->StaminaExhaustionDel.AddUObject(PlayerController, &AMyNewPlayerController::StaminaExhuastion);


	AttackDel.BindUObject(PlayerController->GetPlayerHUD(), &UBaseWidget::UseDamageText);

	CollisionManager->Init();
	CollisionManager->BlockActive(true);

	InitPostProcess();
	RadialBlurOff();

	PlayStart();
}
// Called when the game starts or when spawned
void AMyNewCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitPostProcess();
	RadialBlurOff();
}
void AMyNewCharacter::PlayStart() {
	PlayerController->ChangePlayerState.Broadcast(ENewPlayerState::E_IDLE);
	PlayerController->ChangeActionState.Broadcast(ENewActionState::E_NONE);
	PlayerController->ChangeWeaponType.Broadcast(ENewWeaponType::E_DUAL);
	GhostTrail->InitTrail(10, GetMesh());
}

// Called every frame
void AMyNewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BuffTimer(DeltaTime);
}

void AMyNewCharacter::EndPlay(const EEndPlayReason::Type type) {
	Super::EndPlay(type);
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

void AMyNewCharacter::ChangePlayerState(const ENewPlayerState state) {
	if (CurrentPlayerState == state) return;
	CurrentPlayerState = state;
	switch (CurrentPlayerState)
	{
		case ENewPlayerState::E_IDLE:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			StatusManager->SubStaminaFlag(ENewStaminaState::E_SPRINT);
			break;
		case ENewPlayerState::E_SPRINT:
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			StatusManager->AddStaminaFlag(ENewStaminaState::E_SPRINT);
			break;
		case ENewPlayerState::E_BATTLE:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			StatusManager->SubStaminaFlag(ENewStaminaState::E_SPRINT);
			break;
		case ENewPlayerState::E_DEAD:
			CollisionManager->BlockActive(false);
			GetCharacterMovement()->Deactivate();
			WeaponManager->PlayerDead();
			PlayerController->UnPoessessProcess();
			SetLifeSpan(5.0f);
			break;
	}

}
void AMyNewCharacter::ChangeActionState(const ENewActionState state) {
	if (CurrentActionState == state) return;
	CurrentActionState = state;
	switch (CurrentActionState)
	{
	case ENewActionState::E_NONE:
		CollisionManager->BlockActive(true);
		break;
	case ENewActionState::E_ACTION:
		CollisionManager->BlockActive(true);
		break;
	case ENewActionState::E_HIT:
		CollisionManager->BlockActive(false);
		break;
	case ENewActionState::E_KNOCKBACK:
		CollisionManager->BlockActive(false);
		break;
	case ENewActionState::E_DOWN:
		CollisionManager->BlockActive(false);
		break;
	case ENewActionState::E_EVADE:
		CollisionManager->BlockActive(true);
		IsEvadeFlag = true;
		break;
	}
}
void AMyNewCharacter::ChangeWeaponState(const ENewWeaponType weapon) {
	if (CurrentWeaponType == weapon) return;
	CurrentWeaponType = weapon;
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

	SpawnSlashVFX(Hit, IsCritical);
	PlaySlashSFX();

	if (FinalDamage <= 0) return;

	AttackDel.ExecuteIfBound(Hit.ImpactPoint, FinalDamage, IsWeak);
}
int32 AMyNewCharacter::TakeAttack(const FHitResult& Hit, float damage, float condDamage, class ACharacter* damageCauser, bool& IsWeak, ENewMonsterDamageType type, float knockback)
{
	FVector KnockBackDirection = FVector(-Hit.ImpactNormal.X, -Hit.ImpactNormal.Y,0);
	KnockBackDirection.Normalize();

	if (IsEvadeFlag) {
		MakeBuff(ENewBuffType::E_EVADE, 2.0f);
		MakeBuff(ENewBuffType::E_TIMEDELAY, 0.1f);
		GhostTrail->MakeTrail(GhostTrailRunTime, GhostTrailRemainTime);
		IsEvadeFlag = false;
		return 0;
	}
	float AddForce = 200000;

	float KnockbackValue = knockback;
	switch (type)
	{
	case ENewMonsterDamageType::E_NORMAL:
		//노말로 피격 방향 판단
		PlayerController->ChangeActionState.Broadcast(ENewActionState::E_HIT);
		PlayerController->GetCameraManager()->ShakeCamera(0.2f);
		AnimInst->HitEvent(KnockBackDirection, GetActorRightVector());
		GetCharacterMovement()->AddImpulse(KnockBackDirection*AddForce);
		MakeBuff(ENewBuffType::E_EVADE, 1.0f);
		break;
	case ENewMonsterDamageType::E_KNOCKBACK:
		PlayerController->ChangeActionState.Broadcast(ENewActionState::E_KNOCKBACK);
		PlayerController->GetCameraManager()->ShakeCamera();
		AnimInst->PlayKnockBack();
		LaunchCharacter((KnockBackDirection*2*KnockbackValue)+FVector(0, 0, 300), true, true);
		MakeBuff(ENewBuffType::E_EVADE, 2.5f);
		break;
	case ENewMonsterDamageType::E_ROAR:
		{
			PlayerController->ChangeActionState.Broadcast(ENewActionState::E_HIT);
			AnimInst->PlayRoarHit();
			FRotator Roatate = UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), -KnockBackDirection);
			SetActorRotation(Roatate.Quaternion());
			MakeBuff(ENewBuffType::E_RADIALBLUR, 1.0f);
		}
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
		MakeBuff(ENewBuffType::E_DUALDAMAGE, 1.0f);
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
void AMyNewCharacter::ZeroGroundFriction() {
	GetCharacterMovement()->GroundFriction = 0;
}
void AMyNewCharacter::ResetGroundFriction() {
	GetCharacterMovement()->GroundFriction = GroundFriction;
}
void AMyNewCharacter::RadialBlurOn() {
	BlurMatDynamic->SetScalarParameterValue("Blur", 1.0f);
}
void AMyNewCharacter::RadialBlurOff() {
	BlurMatDynamic->SetScalarParameterValue("Blur", 0.0f);
}
void AMyNewCharacter::InitPostProcess() {
	if (BlurMat != nullptr) {
		BlurMatDynamic = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, BlurMat);
		FWeightedBlendable WeightedBlendable;
		WeightedBlendable.Object = BlurMatDynamic;
		WeightedBlendable.Weight = 1;

		PostProcessMat->Settings.WeightedBlendables.Array.Add(WeightedBlendable);
	}
}

void AMyNewCharacter::MakeBuff(ENewBuffType type, float time) {
	TSharedPtr<PlayerBuff> Effect;
	switch (type) {
	case ENewBuffType::E_DUALDAMAGE:
		Effect = TSharedPtr<PlayerBuff>(new DamageBuff(type, time));
		break;
	case ENewBuffType::E_EVADE:
		Effect = TSharedPtr<PlayerBuff>(new EvadeBuff(type, time));
		break;
	case ENewBuffType::E_RADIALBLUR:
		Effect = TSharedPtr<PlayerBuff>(new RadialBlur(type, time));
		break;
	case ENewBuffType::E_TIMEDELAY:
		Effect = TSharedPtr<PlayerBuff>(new TimeDilation(type, time));
		break;
	}
	Effect->BeginBuff(*this);
	Buff.Add(Effect);
}

void AMyNewCharacter::SpawnSlashVFX(const FHitResult& Hit,bool IsCritical) {
	UNiagaraSystem* Template = (IsCritical == true) ? CriticalSlash : NormalSlash;
	float scale = (IsCritical == true) ? 2.5f : 2.0f;

	float ForwardLength = UKismetMathLibrary::Dot_VectorVector(GetActorForwardVector(), -Hit.ImpactNormal);
	FVector AttackDirection = Hit.ImpactNormal- UKismetMathLibrary::Abs(ForwardLength) * GetActorForwardVector();
	FRotator Rotate = UKismetMathLibrary::FindLookAtRotation(GetActorRightVector(), AttackDirection);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Template, Hit.ImpactPoint, Rotate, FVector::OneVector*scale, false, true, ENCPoolMethod::AutoRelease);
}
void AMyNewCharacter::PlaySlashSFX() {
	if (SlashSound != nullptr) {
		MainAudio->SetSound(SlashSound);
		MainAudio->Play();
	}
}
