// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterStatus.h"
#include "../NewMonster/MyNewBaseMonster.h"
#include "Templates/SharedPointer.h"

UNewMonsterStatus::UNewMonsterStatus() {

}

void UNewMonsterStatus::SetStatusData(float damage, float hp, UDataTable* partsdata) {
	Damage = damage;
	CurrentHp = MaxHp = hp;

	InitHitBox(partsdata);
	Owner = Cast<AMyNewBaseMonster>(GetOwner());
}

void UNewMonsterStatus::InitHitBox(UDataTable* partsData) {
	TArray<FMonsterPartsData*> PartDataRow;
	partsData->GetAllRows(TEXT("PartDataInit"), PartDataRow);

	auto Monster = Cast<AMyNewBaseMonster>(GetOwner());
	auto MonsterMesh = Monster->GetMesh();
	auto Physics = MonsterMesh->SkeletalMesh->PhysicsAsset;
	auto body = Physics->SkeletalBodySetups;

	TArray<FName> Parts;

	TArray<TSharedPtr<MonsterPartsDataInGame>> DataArray;
	for (auto element : body) {
		Parts.Add(element->BoneName);
	}
	for (int i = 0; i < PartDataRow.Num(); ++i) {
		DataArray.Add(TSharedPtr<MonsterPartsDataInGame>(new MonsterPartsDataInGame()));
	}

	for (int i = 0; i < PartDataRow.Num(); ++i) {
		TArray<FName> SearchArray;

		DataArray[i].Get()->SetCauseConditionLimit(MaxHp / 15.0f);
		DataArray[i].Get()->Init(*PartDataRow[i]);

		SearchChildBone(MonsterMesh, PartDataRow[i]->PartRootBoneName, Parts, DataArray[i], SearchArray);
	}
	
}
void UNewMonsterStatus::SearchChildBone(USkeletalMeshComponent* mesh, FName CurrentBone,const TArray<FName>& BonesInBody, TSharedPtr<MonsterPartsDataInGame>& HitBoxData, TArray<FName> &SearchArray) {
	USkeleton* Skeleton = mesh->SkeletalMesh->Skeleton;

	if (Skeleton == nullptr) return;

	int currentIndex = mesh->GetBoneIndex(CurrentBone);

	TArray <int32> Children;
	Skeleton->GetChildBones(currentIndex, Children);


	if (BonesInBody.Contains(CurrentBone)) {
		if (!HitBox.Contains(CurrentBone)) {
			HitBoxData.Get()->Bones.Add(CurrentBone);
			HitBox.Add(CurrentBone, HitBoxData);
			HitBoxPart.Add(HitBoxData.Get()->Type, HitBoxData);
		}
	}

	SearchArray.Add(CurrentBone);

	if (Children.Num() == 0) {
		return;
	}

	for (auto child : Children) {
		FName NextBone = mesh->GetBoneName(child);
		if (!SearchArray.Contains(NextBone)) {
			SearchChildBone(mesh, NextBone, BonesInBody, HitBoxData, SearchArray);
		}
	}

}

int32 UNewMonsterStatus::TakeDamage(const FHitResult& hit, float damage, float condDamage, bool& IsWeak) 
{
	if (Cast<INewDamageInterface>(Owner)->IsAlive() == false) return -1;

	MonsterPartsDataInGame* TargetPart = HitBox[hit.BoneName].Get();
	float Defence = TargetPart->Defence;
	float DefenceRate = Defence / 100.0f;
	const int32 FinalDamage = FMath::FloorToInt(damage * (1 - DefenceRate));
	const int32 FinalCondDamage = FMath::FloorToInt(condDamage * (1 - DefenceRate));

	TargetPart->AccruedDamage += FinalDamage;
	TargetPart->AccruedConditionDamage += FinalCondDamage;

	IsWeak = TargetPart->IsWeak;

	CurrentHp -= FinalDamage;
	if (CurrentHp <= 0) {
		MonsterDeadDel.ExecuteIfBound();
	}

	return FinalDamage;
}

bool UNewMonsterStatus::CheckBreakPart(MonsterPartsDataInGame& target){
	if ((target.CanDestory == true) && (target.IsBreak == false)) {
		if (target.AccruedDamage >= target.DestroyLimit) {

			return true;
		}
	}
	return false;
}
bool UNewMonsterStatus::CheckConditionPart(MonsterPartsDataInGame& target){
	if (target.AccruedConditionDamage >= target.CauseConditionLimit) {
		target.AccruedConditionDamage = 0;
		float NewValue = (target.CauseConditionLimit) * 1.5f;
		target.SetCauseConditionLimit(NewValue);

		//부위파괴 발생 경직
		//모든 부위의 상태이상 데미지 누적치를 절반으로

		//상태이상 발생
		return true;
	}
	return false;
}

void UNewMonsterStatus::SetBroken(ENewMonsterPartsType type) {
	MonsterPartsDataInGame* target = HitBoxPart[type].Get();

	target->IsBreak = true;
	target->Defence = FMath::FloorToInt(target->Defence / 2);
	target->AccruedConditionDamage += (target->DestroyLimit / 2);
}


const MonsterPartsDataInGame& MonsterPartsDataInGame::Init(const FMonsterPartsData& rhs) {
	Type = rhs.Type;
	ConditionType = rhs.ConditionType;

	AccruedDamage = 0;
	AccruedConditionDamage = 0;

	CanDestory = rhs.CanDestory;
	IsBreak = false;
	IsWeak = rhs.IsWeak;
	DestroyLimit = rhs.DestroyDamage;
	Defence = rhs.Defence;

	return *this;
}

void MonsterPartsDataInGame::SetCauseConditionLimit(float value) {
	CauseConditionLimit = value;
}

