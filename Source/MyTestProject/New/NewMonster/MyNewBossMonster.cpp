// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewBossMonster.h"
#include "../NewMonsterComponents/NewBossrStatus.h"
#include "../NewMonsterComponents/NewMonsterProjectile.h"
#include "../NewConditionState/NewConditionState.h"
#include "../NewMonster/NewMonsterController.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "Templates/SharedPointer.h"


AMyNewBossMonster::AMyNewBossMonster() {
	StatusManager = CreateDefaultSubobject<UNewBossrStatus>("StatusManager");
	MonsterSightLength = 3000.0f;
}

void AMyNewBossMonster::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CheckConditionState(DeltaTime);
}

void AMyNewBossMonster::SetCondition(EMonsterConditionState type, float totalTime) {
	TSharedPtr<NewConditionState> Smart = TSharedPtr<NewConditionState>(new NewConditionState(type, totalTime, *MonsterController));
	Smart.Get()->BeginState();
	ConditionStateArray.Emplace(Smart);
}

void AMyNewBossMonster::CheckConditionState(float delta) {
	if (ConditionStateArray.Num() > 0) {
		for (int i = 0; i < ConditionStateArray.Num();) {
			if (ConditionStateArray[i].Get()->GetIsValid()) {
				ConditionStateArray[i].Get()->TickState(delta);
				i++;
			}
			else {
				ConditionStateArray.RemoveAt(i);
			}
		}
	}
}
void AMyNewBossMonster::SetBrokenPart(ENewMonsterPartsType part) {
	int32 brokenPart = 1 << (int32)part;

	if ((BrokenState&brokenPart) != 0) return;
	switch ((ENewMonsterBrokenPart)brokenPart)
	{
	case ENewMonsterBrokenPart::E_HEAD:
		break;
	case ENewMonsterBrokenPart::E_BODY:
		break;
	case ENewMonsterBrokenPart::E_LEFTHAND:
		StatusManager->SetBroken(ENewMonsterPartsType::E_LEFTHAND);
		StatusManager->SetBroken(ENewMonsterPartsType::E_RIGHTHAND);
		MonsterController->SetBrokenState(ENewMonsterBrokenPart::E_LEFTHAND);
		MonsterController->SetBrokenState(ENewMonsterBrokenPart::E_RIGHTHAND);
		break;
	case ENewMonsterBrokenPart::E_RIGHTHAND:
		StatusManager->SetBroken(ENewMonsterPartsType::E_LEFTHAND);
		StatusManager->SetBroken(ENewMonsterPartsType::E_RIGHTHAND);
		MonsterController->SetBrokenState(ENewMonsterBrokenPart::E_LEFTHAND);
		MonsterController->SetBrokenState(ENewMonsterBrokenPart::E_RIGHTHAND);
		break;
	case ENewMonsterBrokenPart::E_LEFTLEG:
		break;
	case ENewMonsterBrokenPart::E_RIGHTLEG:
		break;
	case ENewMonsterBrokenPart::E_TAIL:
		break;
	}
	MonsterController->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_KNOCKBACK);
}

void AMyNewBossMonster::RadialAttack(float Range) {

	TArray<FHitResult> DetectResult;
	FCollisionQueryParams params(NAME_None, false, this);

	bool bHit = GetWorld()->SweepMultiByProfile(
		DetectResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		TEXT("MonsterDamage"),
		FCollisionShape::MakeSphere(Range),
		params);

	//Todo : ControllingPawn Play Sound Roar

	bool IsWeak = false;
	int32 OutFinalDamage = 0;

	TArray<AActor*> repeatedActor;

	for (auto DetectCharacter : DetectResult) {

		//auto Character = Cast<IDamageInterface>(DetectCharacter.Actor);

		/*if (Character != nullptr && !repeatedActor.Contains(DetectCharacter.GetActor())) {
			//Character->TakeDamageFunc(IsWeak, OutFinalDamage, this, DetectCharacter, 0, EDamageType::E_ROAR, NULL);
			//repeatedActor.Add(DetectCharacter.GetActor());
		}*/
	}
}
ANewMonsterProjectile* AMyNewBossMonster::SpawnProjectile(float Range, float damageRate, float Knockback) {
	ANewMonsterProjectile* Projectile = nullptr;
	if (ProjectileType == nullptr) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	FRotator ProjectileMuzzleRotation = FRotator::ZeroRotator; 
	FVector ProjectileMuzzleLocation = (GetMesh()->GetSocketLocation("Muzzle_01") + GetMesh()->GetSocketLocation("Muzzle_02")) / 2;

	Projectile = GetWorld()->SpawnActor<ANewMonsterProjectile>(ProjectileType, ProjectileMuzzleLocation, ProjectileMuzzleRotation, SpawnParams);
	Projectile->SpawnRock(StatusManager->GetDamage()*damageRate, Range, this, ENewMonsterDamageType::E_KNOCKBACK, Knockback);
	
	return Projectile;
}
void AMyNewBossMonster::ShootProjectile(ANewMonsterProjectile* rock) {
	FVector Direction = (MonsterController->GetTarget()->GetActorLocation()) - rock->GetActorLocation();
	rock->SetActorLocation(rock->GetActorLocation() + FVector(0, 0, 100.0f));
	Direction.Normalize();
	FVector LaunchDirection = Direction;
	rock->ThrowRock(LaunchDirection);
}