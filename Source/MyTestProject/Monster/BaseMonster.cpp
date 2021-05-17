// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "MonsterAIController.h"
#include "../Player/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "../Collision/HitCollisionManager.h"
#include "../Components/MonsterStatusManager.h"
#include "../Components/SoundEffectComponent.h"
#include "../Collision/BossCollisionManager.h"
#include "MonsterAnimInstance.h"
#include "Perception/PawnSensingComponent.h"
#include "PhysicsEngine/AggregateGeom.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));

	HitManager = CreateDefaultSubobject<UHitCollisionManager>(TEXT("HITMANAGER"));
	Status = CreateDefaultSubobject<UMonsterStatusManager>(TEXT("STATUS"));

	SoundEffectComp = CreateDefaultSubobject<USoundEffectComponent>(TEXT("SOUNDEFFECTCOMP"));
	Detect = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Detect"));

	PlayerDetectDistance = 2500.0f;
	RoarRange = 3500.0f;

	Detect->bHearNoises = false;
	Detect->bOnlySensePlayers = true;
	Detect->SightRadius = PlayerDetectDistance;
	Detect->SensingInterval = 3.0f;
	Detect->SetPeripheralVisionAngle(85.f);
	Detect->OnSeePawn.AddDynamic(this, &ABaseMonster::OnSeePawn);

	HitRadius = 0.0f;

	CurrentState = EMonsterState::E_CREATE;

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Tags.Add(MonsterTag);
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionProfileName("MonsterHitBox");
	TESTLOG(Warning, TEXT("Monster Begin"));

}

// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ABaseMonster::SetBrokenState(EMonsterPartsType brokenPart) {
	EMonsterBrokenParts part = EMonsterBrokenParts::E_NONE;
	switch (brokenPart)
	{
	case EMonsterPartsType::E_HEAD:
		part = EMonsterBrokenParts::E_HEAD;
		BrokenState = BrokenState | (int32)part;
		break;
	case EMonsterPartsType::E_BODY:
		part = EMonsterBrokenParts::E_BODY;
		BrokenState = BrokenState | (int32)part;
		break;
	case EMonsterPartsType::E_LEFTHAND:
		part = EMonsterBrokenParts::E_LEFTHAND;
		BrokenState = BrokenState | (int32)part;
		break;
	case EMonsterPartsType::E_RIGHTHAND:
		part = EMonsterBrokenParts::E_RIGHTHAND;
		BrokenState = BrokenState | (int32)part;
		break;
	case EMonsterPartsType::E_WING:
		part = EMonsterBrokenParts::E_WING;
		BrokenState = BrokenState | (int32)part;
		break;
	case EMonsterPartsType::E_LEFTLEG:
		part = EMonsterBrokenParts::E_LEFTLEG;
		BrokenState = BrokenState | (int32)part;
		break;
	case EMonsterPartsType::E_RIGHTLEG:
		part = EMonsterBrokenParts::E_RIGHTLEG;
		BrokenState = BrokenState | (int32)part;
		break;
	case EMonsterPartsType::E_TAIL:
		part = EMonsterBrokenParts::E_TAIL;
		BrokenState = BrokenState | (int32)part;
		break;
	}
}
bool ABaseMonster::GetIsAlive() {
	if (CurrentState != EMonsterState::E_DEAD) {
		return true;
	}
	else {
		return false;
	}
}
float ABaseMonster::GetAttackRange() {
	return AttackRange;
}
void ABaseMonster::SetActiveArea(class AMonsterArea* area) {
	ActiveArea = area;
}
AMonsterArea* ABaseMonster::GetActiveArea() {
	return ActiveArea;
}
UHitCollisionManager* ABaseMonster::GetCollisionManager() {
	return HitManager;
}
FString ABaseMonster::GetMonsterName()const {
	return MonsterName;
}
uint8 ABaseMonster::GetMonsterID() const {
	return MonsterID;
}
EMonsterState ABaseMonster::GetMonsterState() {
	return CurrentState;
}
int32 ABaseMonster::GetBrokenState() const {
	return BrokenState;
}
AMonsterAIController* ABaseMonster::GetMonsterController() {
	return MonsterController;
}
UMonsterAnimInstance* ABaseMonster::GetAnimInst() {
	return AnimInst;
}
float ABaseMonster::GetRoarRange() {
	return RoarRange;
}
ABaseCharacter* ABaseMonster::GetTarget() {
	return Target;
}
float ABaseMonster::GetDetectRange() {
	return PlayerDetectDistance;
}
UPawnSensingComponent* ABaseMonster::GetPawnSensing() {
	return Detect;
}
float ABaseMonster::GetDamage() const {
	return Status->GetDamage();
}
void ABaseMonster::ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType, const float ImpactForce) {
	auto TargetActor = hit.Actor;

	bool IsWeak = false;

	float Damage = Status->GetDamage();
	int32 CaculateDamage = 0;

	Damage *= AcitonDamageRate;
	Cast<IDamageInterface>(TargetActor)->TakeDamageFunc(IsWeak, CaculateDamage, this, hit, Damage, DamageType, ImpactForce);
}
void ABaseMonster::TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType, const float ImpactForce) {
	HitManager->ReceiveDamage(hit, hit.BoneName, CaculateDamage, OutFinalDamage, OutIsWeak);
	if (OutFinalDamage == 0) {
		TESTLOG(Warning, TEXT("Bone : %s, Component : %s"), *(hit.BoneName.ToString()), *(hit.Component->GetName()))
	}
	Status->TakeDamage(OutFinalDamage);

	if (CurrentState == EMonsterState::E_IDLE
		&& Target == nullptr) {
		Target = Cast<ABaseCharacter>(Causer);
		MonsterController->SetTarget(Target);
		MonsterController->ChangeMonsterState(EMonsterState::E_BATTLE);
	}
}

void ABaseMonster::ResetDamagedPlayer() {
	DamagedCharacter.Empty();
}

void ABaseMonster::SetMeleeDamage(float damageRate, float force, EDamageType type, EMonsterPartsType EnablePart) {
	DamageRate = damageRate;
	KnockBackForce = force;
	AttackDamageType = type;
	AttackEnablePart = EnablePart;
}

void ABaseMonster::OnSeePawn(APawn *OtherPawn) {
	if (OtherPawn != nullptr && CurrentState == EMonsterState::E_IDLE) {
		auto temp = Cast<ABaseCharacter>(OtherPawn);
		if (temp->GetState() != ECharacterState::E_DEAD) {
			Target = temp;
			MonsterController->SetTarget(Target);
			MonsterController->ChangeMonsterState(EMonsterState::E_BATTLE);
		}
		else {
			Target = nullptr;
			MonsterController->SetTarget(Target);
			MonsterController->ChangeMonsterState(EMonsterState::E_IDLE);
		}
	}
}
 bool ABaseMonster::MonsterInit(FMonsterStatus status) {
	 auto DataTable = status.PartsDataRef;

	 MonsterName = status.MonsterName;
	 if (DataTable != nullptr && HitManager != nullptr) {
		 HitManager->InitHitBox(DataTable, GetMesh());
	 }
	 Status->SetStatus(status.HP, status.Damage);
	 Status->DeadDel.AddUObject(this, &ABaseMonster::Dead);

	 LeftHand = status.LeftHandSocket;
	 RightHand = status.RightHandSocket;
	 HitRadius = status.Radius;

	 MonsterController = Cast<AMonsterAIController>(GetController());
	 MonsterController->StateChangeDel.AddUObject(this, &ABaseMonster::ChangeMonsterState);
	 MonsterController->StateChangeDel.AddUObject(AnimInst, &UMonsterAnimInstance::ChangeMonsterState);
	 MonsterController->StateChangeDel.Broadcast(EMonsterState::E_IDLE);

	 GetMesh()->SetScalarParameterValueOnMaterials("ScarScale", 0.0f);

	 return true;
}
void ABaseMonster::HitCheck(EMonsterPartsType Part, float damageRate, float knockbackDist, EDamageType damagetype) {

	TArray<FHitResult> DetectResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	TArray<AActor*> Ignore;
	Ignore.Add(this);

	FName TargetPart;
	
	if (Part == EMonsterPartsType::E_LEFTHAND) {
		TargetPart = LeftHand;
	}
	else {
		TargetPart = RightHand;
	}

	float scale = 1.5f;

	bool Result = GetWorld()->SweepMultiByProfile(
		DetectResult,
		GetMesh()->GetSocketLocation(TargetPart),
		GetMesh()->GetSocketLocation(TargetPart),
		FRotator::ZeroRotator.Quaternion(),
		TEXT("MonsterDamage"), FCollisionShape::MakeSphere(HitRadius), Params
	);

	DrawDebugSphere(GetWorld(), GetMesh()->GetSocketLocation(TargetPart), HitRadius, 30, FColor::Red, false, 0.5f);
	
	/*UKismetSystemLibrary::CapsuleTraceMultiByProfile(GetWorld(), Location, Location,CollisionShape.Radius * 2,
		(CollisionShape.Radius + CollisionShape.Length) * 2, TEXT("MonsterDamage"), true, Ignore, EDrawDebugTrace::ForDuration, DetectResult, true, FLinearColor::Blue, FLinearColor::Green, 1.0f);
		*/

	bool IsWeak = false;
	int32 OutFinalDamage = 0;

	TArray<AActor*> repeatedActor;

	if (!Result) return;
	//if (DetectResult.Num() <= 0)return;

	TESTLOG(Warning, TEXT("test"));
	for (auto DetectCharacter : DetectResult) {

		auto Character = Cast<IDamageInterface>(DetectCharacter.Actor);

		if (Character != nullptr && !repeatedActor.Contains(DetectCharacter.GetActor())) {
			TESTLOG(Warning, TEXT("%s"), *(DetectCharacter.Actor->GetName()));
			Character->TakeDamageFunc(IsWeak, OutFinalDamage, this, DetectCharacter, DamageRate*Status->GetDamage(), damagetype, knockbackDist);
			repeatedActor.Add(DetectCharacter.GetActor());
		}
	}
}

void ABaseMonster::Dead() {

	MonsterController->ChangeMonsterState(EMonsterState::E_DEAD);
}