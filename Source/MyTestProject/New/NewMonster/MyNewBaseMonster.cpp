// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewBaseMonster.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../NewMonsterComponents/NewMonsterStatus.h"
#include "NewMonsterController.h"
#include "NewMonsterAnimInstance.h"
#include "PhysicsEngine/SphylElem.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/PawnSensingComponent.h"
#include "NewMonsterSpawner.h"
#include "../NewMonsterComponents/NewMonsterProjectile.h"

// Sets default values
AMyNewBaseMonster::AMyNewBaseMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Monster")));
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetCapsuleComponent()->bDynamicObstacle = true;

	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -95.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetMesh()->SetCollisionProfileName("MonsterHitBox");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	RunSpeed = 800;
	WalkSpeed = 300;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	GetCharacterMovement()->MaxAcceleration = RunSpeed;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	CurrentMonsterState = ENewMonsterState::E_CREATE;

	AIControllerClass = ANewMonsterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	MonsterSight = CreateDefaultSubobject<UPawnSensingComponent>("MonsterSight");

	MonsterSight->bHearNoises = false;
	MonsterSight->bOnlySensePlayers = true;;
	MonsterSight->SensingInterval = 1.0f;
	MonsterSight->SetPeripheralVisionAngle(85.f);

}


void AMyNewBaseMonster::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	MonsterController = Cast<ANewMonsterController>(NewController);

	MonsterController->ChangeStateDel.AddUObject(this, &AMyNewBaseMonster::ChangeMonsterState);
	MonsterController->ChangeConditionStateDel.AddUObject(this, &AMyNewBaseMonster::ChangeMonsterConditionState);

	StatusManager->MonsterDeadDel.BindUObject(MonsterController, &ANewMonsterController::MonsterDead);

}
void AMyNewBaseMonster::PostInitializeComponents() {
	Super::PostInitializeComponents();
}
// Called when the game starts or when spawned
void AMyNewBaseMonster::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyNewBaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyNewBaseMonster::SetUpMonster(const FNewMonsterData& data, ANewMonsterSpawner* area) {
	MonsterID = data.MonsterID;

	MonsterArea = area;

	GetMesh()->SetSkeletalMesh(data.Mesh);
	MonsterSize = data.Size;
	GetMesh()->SetRelativeScale3D(FVector::OneVector * MonsterSize);
	GetMesh()->SetAnimInstanceClass(data.MonsterAnim);

	MonsterController->SetData(data.MonsterAI,*data.BlackBoard, *this, *MonsterArea, MonsterID);

	StatusManager->SetStatusData(data.Damage, data.Hp, data.PartsData);

	if (data.Projectile != nullptr) {
		ProjectileType = data.Projectile;
	}

	AnimInst = Cast<UNewMonsterAnimInstance>(GetMesh()->GetAnimInstance());

	MonsterSight->SightRadius = MonsterSightLength;
	MonsterSight->OnSeePawn.AddDynamic(this, &AMyNewBaseMonster::OnFindPlayer);

	MonsterController->ChangeStateDel.AddUObject(AnimInst, &UNewMonsterAnimInstance::ChangeMonsterState);
	MonsterController->ChangeConditionStateDel.AddUObject(AnimInst, &UNewMonsterAnimInstance::ChangeMonsterConditionState);

	MonsterController->ChangeStateDel.Broadcast(ENewMonsterState::E_IDLE);
	MonsterController->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_IDLE);
}

void AMyNewBaseMonster::ChangeMonsterState(const ENewMonsterState state) {
	if (CurrentMonsterState == state) return;
	CurrentMonsterState = state;
	switch (CurrentMonsterState)
	{
	case ENewMonsterState::E_IDLE:
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		break;
	case ENewMonsterState::E_PLAYERFOUND:
		break;
	case ENewMonsterState::E_BATTLE:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	case ENewMonsterState::E_DEAD:
		SetLifeSpan(10.0f);
		break;
	}
}
void AMyNewBaseMonster::ChangeMonsterConditionState(const EMonsterConditionState state) {
	if (CurrentConditionState == state) return;
	CurrentConditionState = state;
	switch (CurrentConditionState)
	{
	case EMonsterConditionState::E_IDLE:
		break;
	case EMonsterConditionState::E_KNOCKBACK:
		break;
	case EMonsterConditionState::E_STUN:
		break;
	case EMonsterConditionState::E_DOWN:
		break;
	}
}

void AMyNewBaseMonster::AttackCheck(ENewMonsterAttackPart part, float damage, ENewMonsterDamageType type, float knockback) {
	TArray<MonsterPartsDataInGame*> HitBoxes;
	if (part == ENewMonsterAttackPart::E_LEFT) {
		StatusManager->GetHitBoxData(ENewMonsterPartsType::E_LEFTHAND,HitBoxes);
	}
	else if ((part == ENewMonsterAttackPart::E_RIGHT)) {
		StatusManager->GetHitBoxData(ENewMonsterPartsType::E_RIGHTHAND,HitBoxes);
	}
	else {
		StatusManager->GetHitBoxData(ENewMonsterPartsType::E_LEFTHAND, HitBoxes);
		StatusManager->GetHitBoxData(ENewMonsterPartsType::E_RIGHTHAND, HitBoxes);
	}

	auto physics = GetMesh()->SkeletalMesh->PhysicsAsset;
	auto BodyMap = physics->BodySetupIndexMap;
	auto SkeletalBodySetup = physics->SkeletalBodySetups;

	TArray<FName> BoneNames;
	for (auto hitbox : HitBoxes) {
		for (auto bone : hitbox->Bones) {
			BoneNames.Emplace(bone);
		}
	}

	TArray<int32> Bodyindex;
	for (auto name : BoneNames) {
		Bodyindex.Add(BodyMap[name]);
	}
	FTransform Trans;
	for (auto index : Bodyindex) {
		USkeletalBodySetup* CurrentBodySetup = SkeletalBodySetup[index];
		HitCheck(CurrentBodySetup->BoneName, CurrentBodySetup->AggGeom.SphylElems[0], damage*StatusManager->GetDamage(), knockback, type);
	
	}


}
void AMyNewBaseMonster::HitCheck(const FName BoneName,const FKSphylElem& collision, float damage,float knockback, ENewMonsterDamageType type) {
	FVector Location;
	FQuat Rotation;


	FTransform BoneTransform = GetMesh()->GetSocketTransform(BoneName);

	FVector Center = collision.Center;

	FRotator InverseRot = UKismetMathLibrary::InverseTransformRotation(BoneTransform, BoneTransform.Rotator());
	FVector InverseLoc = UKismetMathLibrary::InverseTransformLocation(BoneTransform, BoneTransform.GetLocation());
	FVector Loc = InverseLoc+Center;
	FRotator Rot = UKismetMathLibrary::ComposeRotators(InverseRot, collision.Rotation);
	
	Rot = UKismetMathLibrary::TransformRotation(BoneTransform, Rot);
	Loc = UKismetMathLibrary::TransformLocation(BoneTransform, Loc);

	FRotator rot = InverseRot;
	FRotator BoneLocal = UKismetMathLibrary::ComposeRotators(rot, collision.Rotation);

	Rotation = Rot.Quaternion();
	Location = Loc;

	float CapsuleRadius = collision.Radius*MonsterSize*1.2f;
	float CapsuleLength = collision.Length*MonsterSize*1.2f;

	TArray<FHitResult> DetectResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool Result = GetWorld()->SweepMultiByProfile(
		DetectResult,
		Location,
		Location,
		Rotation,
		TEXT("MonsterDamage"), FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleLength), Params
	);

	UKismetSystemLibrary::DrawDebugCapsule(GetWorld(), Location, CapsuleLength, CapsuleRadius, Rot, FColor::Red, 0.1f, 1.5f);

	bool IsWeak = false;

	if (DetectResult.Num()<=0) return;

	for (auto DetectCharacter : DetectResult) {

		auto Character = Cast<INewDamageInterface>(DetectCharacter.Actor);

		if (Character != nullptr && !DamagedCharacters.Contains(DetectCharacter.GetActor())) {
			Character->TakeAttack(DetectCharacter, damage, NULL, nullptr, IsWeak, type, knockback);
			DamagedCharacters.Add(Cast<AMyNewCharacter>(DetectCharacter.GetActor()));
		}
	}
}
void AMyNewBaseMonster::SetCondition(EMonsterConditionState type, float totalTime){}
void AMyNewBaseMonster::SetBrokenPart(ENewMonsterPartsType brokenpart){}

void AMyNewBaseMonster::ClearDamagedCharacter() {
	DamagedCharacters.Empty();
}
void AMyNewBaseMonster::ApplyAttack(const FHitResult& Hit, float damage, float condDamage, float critical)
{

}
int32 AMyNewBaseMonster::TakeAttack(const FHitResult& Hit, float damage, float condDamage, class ACharacter* damageCauser, bool& IsWeak, ENewMonsterDamageType type, float knockback)
{
	if (CurrentMonsterState == ENewMonsterState::E_IDLE) {
		auto Target = Cast<AMyNewCharacter>(damageCauser);
		MonsterController->SetTarget(Target);
		MonsterController->ChangeStateDel.Broadcast(ENewMonsterState::E_PLAYERFOUND);
	}

	return StatusManager->TakeDamage(Hit, damage, condDamage, IsWeak);
}
bool AMyNewBaseMonster::IsAlive()
{
	if (CurrentMonsterState != ENewMonsterState::E_DEAD) {
		return true;
	}
	return false;
}

void AMyNewBaseMonster::OnFindPlayer(APawn* pawn) {
	auto Player = Cast<AMyNewCharacter>(pawn);
	if (IsValid(Player)) {
		if (Player->IsAlive() && MonsterArea->GetIsPlayerInArea()) {
			MonsterController->SetTarget(Player);
		}
	}
}
