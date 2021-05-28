// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusComponent.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../NewWeapon/MyNewWeaponManager.h"
#include "../NewWeapon/MyNewBaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "../MyNewGameInstance.h"

UCharacterStatusComponent::UCharacterStatusComponent() {
	WeaponDamage = 0.0f;
	OperandField = 1.0f;
	Critical = 0.0f;
}
void UCharacterStatusComponent::BeginPlay() {
	Super::BeginPlay();

}

void UCharacterStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StaminaFlag == (int)ENewStaminaState::E_IDLE) {
		StaminaRegen(DeltaTime);
	}

	if ((StaminaFlag&(int)ENewStaminaState::E_SPRINT) == true) {
		UseStamina(3.0f, DeltaTime);
	}

	UpdateStatus();
}
void UCharacterStatusComponent::InitStatus(APawn* owner) {
	Owner = Cast<AMyNewCharacter>(owner);
	
	auto PlayerData = &Cast<UMyNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetPlayerDataTable();
	auto Names = PlayerData->GetRowNames();
	
	auto Data = PlayerData->FindRow<FNewPlayerStatus>(Names[0], TEXT("Player Status Init"), true);

	MaxHp = Data->MaxHp;
	MaxStamina = Data->MaxStamina;

	CurrentHp = MaxHp;
	CurrentStamina = MaxStamina;

}
void UCharacterStatusComponent::StaminaRegen(float delta) {
	if (CurrentStamina <= MaxStamina) {
		CurrentStamina += 5.0f*delta;
	}
	else {
		CurrentStamina = MaxStamina;
	}
}

void UCharacterStatusComponent::UpdateStatus() {
	for (int i = 0; i < Observers.Num(); ++i) {
		Observers[i]->NotifyStatus(MaxHp, CurrentHp, MaxStamina, CurrentStamina);
	}
}
void UCharacterStatusComponent::ChangeWeaponState(const ENewWeaponType weapon) {
	auto TempWeapon = Owner->GetWeaponManager()->GetCurrentWeapon();
	WeaponDamage = TempWeapon->GetDamage();
	Critical = TempWeapon->GetCritical();
}
bool UCharacterStatusComponent::GetCalculatedDamage(float& outDamage) {
	int rand = FMath::RandRange(0, 99);

	float CalculatedDamage = WeaponDamage * OperandField;

	if (rand <= Critical) {
		CalculatedDamage *= 1.5f;
		outDamage = CalculatedDamage;
		return true;
	}
	else {
		outDamage = CalculatedDamage;
		return false;
	}
}

bool UCharacterStatusComponent::CheckStamina(float amount) {
	if (CurrentStamina >= amount) {
		return true;
	}
	return false;
}
bool UCharacterStatusComponent::UseStamina(float amount) {
	if (CheckStamina(amount)) {
		CurrentStamina -= amount;
		return true;
	}
	return false;
}

void UCharacterStatusComponent::Attach(IUpdateStatus* observer) {
	Observers.Add(observer);
}

void UCharacterStatusComponent::UseStamina(float amountPerSeconds, float delta) {
	auto amountPerDelta = amountPerSeconds * delta;
	if (CurrentStamina - amountPerDelta >= 0) {
		CurrentStamina -= amountPerDelta;
	}
	else {
		CurrentStamina = 0.0f;
		StaminaExhaustionDel.Broadcast();
		StaminaFlag = (int)ENewStaminaState::E_IDLE;
	}
}
void UCharacterStatusComponent::RegenHp(float amountPerSeconds, float delta) {
	auto amountPerDelta = amountPerSeconds * delta;
	if (CurrentHp + amountPerDelta <= MaxHp) {
		CurrentHp += amountPerDelta;
	}
	else {
		CurrentHp = MaxHp;
	}
}
void UCharacterStatusComponent::TakeDamage(float Damage) {
	CurrentHp -= Damage;
	if (CurrentHp <= 0) {
		PlayerDeadDel.ExecuteIfBound();
	}
	UpdateStatus();
}

void UCharacterStatusComponent::SetWeaponData(float damage, float conddamage, float critical) {
	WeaponDamage = damage;
	WeaponCondDamage = conddamage;
	Critical = critical;
}