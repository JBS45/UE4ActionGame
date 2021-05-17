// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectClass.h"
#include "GameFramework/Actor.h"
#include "Player/BaseCharacter.h"
#include "Player/BasePlayerController.h"
#include "Components/MonsterStatusManager.h"
#include "Components/CharacterStatusManager.h"
#include "Input/InputBufferManager.h"
#include "Monster/BaseMonster.h"
#include "Monster/MonsterAIController.h"

UEffectClass::UEffectClass()
{

}
EEffectType UEffectClass::GetIdentifier() {
	return Identifier;
}

UDualSpecial::UDualSpecial() {

}

void UDualSpecial::BeginEffect(AActor* owner)
{
	Owner = owner;
	Identifier = EEffectType::E_DUALSPECIAL;

	Character = Cast<ABaseCharacter>(Owner);
	DelegateHandle = Character->GetCharacterStatus()->OnStaminaZeroDel.AddUObject(this, &UDualSpecial::EndEffect);
	Character->GetCharacterStatus()->StaminaUse(ECharacterStaminaUse::E_SPECIAL, true);
	Character->SetMaterialMesh(true);
}
void UDualSpecial::TickEffect(float delta)
{
	Character->GetCharacterStatus()->ConsumeStamina(5.0f, delta);
	Character->GetCharacterStatus()->SetBuffRate(1.5f);
}
void UDualSpecial::EndEffect()
{
	Character->GetCharacterStatus()->SetBuffRate(1.0f);
	Character->GetCharacterStatus()->OnStaminaZeroDel.Remove(DelegateHandle);
	Character->GetCharacterStatus()->StaminaUse(ECharacterStaminaUse::E_SPECIAL, false);
	Cast<ABasePlayerController>(Character->GetController())->GetInputBuffer()->SpecialOff();
	Character->SetMaterialMesh(false);
	Identifier = EEffectType::E_INVALID;
}