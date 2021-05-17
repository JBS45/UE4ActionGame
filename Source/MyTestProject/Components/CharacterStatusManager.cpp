// Fill out your copyright notice in the Description page of Project Settings.




#include "CharacterStatusManager.h"

// Sets default values
UCharacterStatusManager::UCharacterStatusManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void UCharacterStatusManager::BeginPlay()
{
	Super::BeginPlay();
	IsAlive = true;
}

// Called every frame
void UCharacterStatusManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (StaminaUseFlag == 0) {
		RegenStamina(DeltaTime);
	}
}
void UCharacterStatusManager::StatusInit() {
	CreateCharState();

	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEquipmentState"), true);
	if (enumPtr != nullptr) {
		for (int i = 0; i < enumPtr->NumEnums(); ++i) {
			EEquipmentState Key = (EEquipmentState)enumPtr->GetValueByIndex(i);
			FEquipmentStatus Value;
			CharacterStatus.Slot.Add(Key, Value);
		}
	}

	UpdateStatus();

	CharacterStatus.HpRegen = 2.0f;
	CharacterStatus.StaminaRegen = 15.0f;
	CharacterStatus.CurrentHp = CharacterStatus.MaxHp;
	CharacterStatus.CurrentStamina = CharacterStatus.MaxStamina;

}
void UCharacterStatusManager::CreateCharState() {
	CharacterStatus.CharacterStatus.Hp = 100.0f;
	CharacterStatus.CharacterStatus.Stamina = 100.0f;
	CharacterStatus.CharacterStatus.Dmg = 10.0f;
	CharacterStatus.CharacterStatus.Def = 0.0f;
	CharacterStatus.CharacterStatus.CriticalRate = 0.0f;

}
void UCharacterStatusManager::EquipItem(const FEquipmentStatus item) {

	auto TargetSlot = CharacterStatus.Slot[item.EquipType];

	if (TargetSlot.IsEmpty) {
		TargetSlot = item;
		TargetSlot.IsEmpty = false;
	}
	else {
		//TODO : EXCHANGE ITEM DATA
	}

	UpdateStatus();
}
void UCharacterStatusManager::UpdateStatus() {
	FBaseStatus TempStatus = CharacterStatus.CharacterStatus;

	for (auto slotdata : CharacterStatus.Slot) {
		if (!slotdata.Value.IsEmpty) {
			TempStatus += slotdata.Value.StatusData;
		}
	}

	CharacterStatus.MaxHp = TempStatus.Hp;
	CharacterStatus.MaxStamina = TempStatus.Stamina;
	CharacterStatus.Dmg = TempStatus.Dmg * BuffRate;
	CharacterStatus.Def = TempStatus.Def;
	CharacterStatus.CriticalRate = TempStatus.CriticalRate;

	Notify();
}


int32 UCharacterStatusManager::TakeDamage(float Damage) {
	int32 FinalDamage = 0;

	if (CharacterStatus.Def <= 0) {
		FinalDamage = FMath::FloorToInt(Damage);
	}
	else {
		float DefRate = CharacterStatus.Def / (CharacterStatus.Def + 100);
		FinalDamage = FMath::FloorToInt(Damage*DefRate);
	}
	CharacterStatus.CurrentHp -= FinalDamage;

	if (CharacterStatus.CurrentHp <= 0) {
		OnCharacterDeadDel.Execute();
	}

	Notify();

	return FinalDamage;
}
bool UCharacterStatusManager::EvadeStamina() {
	if (CharacterStatus.CurrentStamina >= CharacterStatus.EvadeUseStaminaAmount) {
		CharacterStatus.CurrentStamina -= CharacterStatus.EvadeUseStaminaAmount;
		Notify();
		return true;
	}
	else {
		return false;
	}
}

void UCharacterStatusManager::ConsumeStamina(float value, float delta) {
	if (CharacterStatus.CurrentStamina > value * (delta / ReGenBaseTime)) {
		CharacterStatus.CurrentStamina -= value * (delta / ReGenBaseTime);
	}
	else {
		CharacterStatus.CurrentStamina = 0;
		StaminaUseFlag = 0;
		OnStaminaZeroDel.Broadcast();
	}
	Notify();
}


void UCharacterStatusManager::RegenStamina(float delta) {
	if (CharacterStatus.CurrentStamina < CharacterStatus.MaxStamina) {
		CharacterStatus.CurrentStamina += CharacterStatus.StaminaRegen*(delta / ReGenBaseTime);
	}
	else {
		CharacterStatus.CurrentStamina = CharacterStatus.MaxStamina;
	}
	Notify();
}


const FBaseStatus FBaseStatus::operator+(const FBaseStatus& rhs) {
	return FBaseStatus(*this) += rhs;
}

FBaseStatus& FBaseStatus::operator+=(const FBaseStatus& rhs) {
	this->Hp += rhs.Hp;
	this->Stamina += rhs.Stamina;
	this->Dmg += rhs.Dmg;
	this->Def += rhs.Def;
	this->CriticalRate += rhs.CriticalRate;
	return *this;
}

void UCharacterStatusManager::Attach(IStatusInterface* newObserver) {
	StatusObservers.Add(newObserver);
}

void UCharacterStatusManager::Notify() {
	for (auto observer : StatusObservers) {
		observer->NotifyStatusData(CharacterStatus);
	}
}

float UCharacterStatusManager::GetDamage() {
	return CharacterStatus.Dmg;
}

float UCharacterStatusManager::GetCritical() {
	return CharacterStatus.CriticalRate;
}

float UCharacterStatusManager::GetConsumeSprint() const {
	return CharacterStatus.StaminaConsumeSprint;
}

void UCharacterStatusManager::SetBuffRate(float value) {
	BuffRate = value;
}

void UCharacterStatusManager::StaminaUse(ECharacterStaminaUse type, bool IsOn) {
	if (IsOn) {
		StaminaUseFlag |= (int32)type;
	}
	else {
		StaminaUseFlag = (~(int32)type)&StaminaUseFlag;
	}
}