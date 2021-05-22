// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBuff.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../PlayerCharacterComponents/CharacterStatusComponent.h"
#include "Kismet/GameplayStatics.h"

PlayerBuff::PlayerBuff(ENewBuffType type, float time) :Type(type), RemainTime(time)
{
}

PlayerBuff::~PlayerBuff()
{
}


DamageBuff::DamageBuff(ENewBuffType type, float time) :PlayerBuff(type, time)
{
}
DamageBuff::~DamageBuff() 
{
}

void DamageBuff::BeginBuff(AMyNewCharacter& character) {
	IsValid = true;
	Owner = &character;
	status = Owner->GetStatusManager();
	status->AddStaminaFlag(ENewStaminaState::E_SPECIAL);
	EndDel = status->StaminaExhaustionDel.AddRaw(this, &DamageBuff::EndBuff);
	Owner->SetMaterailMesh(true);
	Owner->SetParticlce(true);
}
void DamageBuff::TickBuff(float delta) {
	RemainTime -= delta;
	status->SetOperandField(2.0f);
	status->UseStamina(3.0f, delta);
}
void DamageBuff::EndBuff() {
	status->SubStaminaFlag(ENewStaminaState::E_SPECIAL);
	status->StaminaExhaustionDel.Remove(EndDel);
	Owner->SetMaterailMesh(false);
	Owner->SetParticlce(false);
	IsValid = false;
}

EvadeBuff::EvadeBuff(ENewBuffType type, float time) :PlayerBuff(type, time)
{
}
EvadeBuff::~EvadeBuff()
{
}
void EvadeBuff::BeginBuff(AMyNewCharacter& character) {
	IsValid = true;
	Owner = &character;
	Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void EvadeBuff::TickBuff(float delta) {
	RemainTime -= delta;
	if (RemainTime <= 0.0f) {
		EndBuff();
	}
}
void EvadeBuff::EndBuff() {
	Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	IsValid = false;
}
TimeDilation::TimeDilation(ENewBuffType type, float time) :PlayerBuff(type, time)
{
}
TimeDilation::~TimeDilation()
{
}

void TimeDilation::BeginBuff(AMyNewCharacter& character) {
	IsValid = true;
	Owner = &character;
	UGameplayStatics::SetGlobalTimeDilation(Owner->GetWorld(), 0.33);
	//Owner->CustomTimeDilation = 3.0f;
}
void TimeDilation::TickBuff(float delta) {
	RemainTime -= delta;
	if (RemainTime <= 0.0f) {
		EndBuff();
	}
}
void TimeDilation::EndBuff() {
	UGameplayStatics::SetGlobalTimeDilation(Owner->GetWorld(), 1.0);
	Owner->CustomTimeDilation = 1.0f;
	IsValid = false;
}

PotionTimer::PotionTimer() : CanUse(true), RemainTime(0), PotionCoolTime(20.0f)
{
}
PotionTimer::~PotionTimer()
{
}

bool PotionTimer::Use() {
	if (CanUse) {
		RemainTime = PotionCoolTime;
		CanUse = false;
		return true;
	}
	else {
		return false;
	}
}
void PotionTimer::Timer(float delta) {
	if (CanUse == false) {
		RemainTime -= delta;
		if (RemainTime <= 0) {
			CanUse = true;
		}
	}
}