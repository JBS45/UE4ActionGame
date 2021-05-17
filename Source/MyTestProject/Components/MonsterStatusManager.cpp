// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatusManager.h"
#include "../Monster/BaseMonster.h"
#include "../Monster/MonsterAIController.h"
#include "../Monster/Boss.h"
#include "../StunState.h"

// Sets default values for this component's properties
UMonsterStatusManager::UMonsterStatusManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMonsterStatusManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UMonsterStatusManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMonsterStatusManager::SetStatus(float maxHp, float baseDamage)
{
	MaxHP = maxHp;
	CurrentHP = MaxHP;
	BaseDamage = baseDamage;

	BrokenState = (int32)EMonsterBrokenParts::E_NONE;
}
void UMonsterStatusManager::TakeDamage(const int32 acculateDamage) {
	CurrentHP -= (float)acculateDamage;
	StunValue += acculateDamage;
	if (StunValue > MaxHP * 0.25) {
		StunValue = 0;
		auto Rampage = Cast<ABoss>(GetOwner());
		if (Rampage != nullptr) {
			Rampage->GetStunState()->BeginStun(Rampage, 5.0f);
		}
	}
	TESTLOG(Warning, TEXT("Current : %f, MAX : %f"), CurrentHP, MaxHP);
	if (CurrentHP <= HPPERCENT(40)) {
		ChangePhase();
	}
	if (CurrentHP <= 0) {
		DeadDel.Broadcast();
	}
}
void UMonsterStatusManager::SetBrokenState(EMonsterBrokenParts broken)
{
	BrokenState |= (int32)broken;
	auto Rampage = Cast<ABoss>(GetOwner());
	if (Rampage != nullptr) {
		Rampage->GetStunState()->BeginStun(Rampage, 5.0f);
	}
	BrokenDel.Broadcast();
}
float UMonsterStatusManager::GetDamage() {
	return BaseDamage;
}

void UMonsterStatusManager::ChangePhase() {
	auto Controller = Cast<ABaseMonster>(GetOwner())->GetMonsterController();
	Controller->StateChangeDel.Broadcast(EMonsterState::E_RAGE);

}

