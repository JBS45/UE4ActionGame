// Fill out your copyright notice in the Description page of Project Settings.


#include "MyNewWeaponManager.h"
#include "MyNewBaseWeapon.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../MyNewGameInstance.h"
#include "../PlayerCharacterComponents/CharacterStatusComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UMyNewWeaponManager::UMyNewWeaponManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AttachRightDrawSocket = FName("RightHand");
	AttachLeftDrawSocket = FName("LeftHand");

	AttachRightPutUpSocket = FName("RightWeapon");
	AttachLeftPutUpSocket = FName("LeftWeapon");
	AttachSwordPutUpSocket = FName("SwordPutUp");
	AttachAxePutUpSocket = FName("AxePutUp");

	IsInit = false;
}


// Called when the game starts
void UMyNewWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
void UMyNewWeaponManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType,ThisTickFunction);
	if (IsInit) {
		WeaponOwner->GetStatusManager()->SetWeaponData(CurrentWeapon->GetDamage(), CurrentWeapon->GetConditionDamage(), CurrentWeapon->GetCritical());
	}
}

void UMyNewWeaponManager::SetInit(AMyNewCharacter* owner) {
	WeaponOwner = owner;

	UDataTable* TmpTable = Cast<UMyNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetWeaponDataTable();
	auto RowNames = TmpTable->GetRowNames();
	for (auto row : RowNames) {
		static const FString ContextString(TEXT("Set Weapon"));
		auto Data = TmpTable->FindRow<FNewWeaponData>(row, ContextString, true);
		if (Data->Type == ENewWeaponType::E_DUAL) {
			auto WeaponClass = Cast<UClass>(Data->WeaponActor);
			auto WeaponActor1 = GetWorld()->SpawnActor<AMyNewBaseWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
			WeaponActor1->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachRightPutUpSocket);
			WeaponActor1->InitWeapon(*Data, WeaponOwner);
			Weapons.Add(WeaponActor1);

			auto WeaponActor2 = GetWorld()->SpawnActor<AMyNewBaseWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
			WeaponActor2->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachLeftPutUpSocket);
			WeaponActor2->InitWeapon(*Data, WeaponOwner);
			Weapons.Add(WeaponActor2);
		}
		else if(Data->Type == ENewWeaponType::E_KATANA){
			auto WeaponClass = Cast<UClass>(Data->WeaponActor);
			auto WeaponActor = GetWorld()->SpawnActor<AMyNewBaseWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
			WeaponActor->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSwordPutUpSocket);
			WeaponActor->InitWeapon(*Data, WeaponOwner);
			Weapons.Add(WeaponActor);
		}
		else {
			auto WeaponClass = Cast<UClass>(Data->WeaponActor);
			auto WeaponActor = GetWorld()->SpawnActor<AMyNewBaseWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
			WeaponActor->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachAxePutUpSocket);
			WeaponActor->InitWeapon(*Data, WeaponOwner);
			Weapons.Add(WeaponActor);
		}
	}
	for (auto weapon : Weapons) {
		weapon->SetEnable(false);
	}
	IsInit = true;
}

void UMyNewWeaponManager::Draw() {
	switch (CurrentWeaponType) {
		case ENewWeaponType::E_DUAL:
			(Weapons[0])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachLeftDrawSocket);
			(Weapons[1])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachRightDrawSocket);
			(Weapons[0])->AttachDraw();
			(Weapons[1])->AttachDraw();
			(Weapons[0])->SetEnable(true);
			(Weapons[1])->SetEnable(true);
			break;
		case ENewWeaponType::E_KATANA:
			(Weapons[2])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachRightDrawSocket);
			(Weapons[2])->AttachDraw();
			(Weapons[2])->SetEnable(true);
			break;
		case ENewWeaponType::E_AXE:
			/*Axe Draw*/
			(Weapons[3])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachRightDrawSocket);
			(Weapons[3])->AttachDraw();
			(Weapons[3])->SetEnable(true);
			break;
	}
}
void UMyNewWeaponManager::PutUp() {
	switch (CurrentWeaponType) {
	case ENewWeaponType::E_DUAL:
		(Weapons[0])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachLeftPutUpSocket);
		(Weapons[1])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachRightPutUpSocket);
		(Weapons[0])->AttachPutUp();
		(Weapons[1])->AttachPutUp();
		(Weapons[0])->SetEnable(false);
		(Weapons[1])->SetEnable(false);
		break;
	case ENewWeaponType::E_KATANA:
		(Weapons[2])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSwordPutUpSocket);
		(Weapons[2])->AttachPutUp();
		(Weapons[2])->SetEnable(false);
		break;
	case ENewWeaponType::E_AXE:
		(Weapons[3])->AttachToComponent(WeaponOwner->GetMainMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachAxePutUpSocket);
		(Weapons[3])->AttachPutUp();
		(Weapons[3])->SetEnable(false);
		break;
	}
}

void UMyNewWeaponManager::ChangeWeaponState(const ENewWeaponType weapon) {
	if (CurrentWeaponType == weapon) return;

	PutUp();
	CurrentWeaponType = weapon;
	switch (CurrentWeaponType) {
	case ENewWeaponType::E_DUAL:
		CurrentWeapon = Weapons[0];
		break;
	case ENewWeaponType::E_KATANA:
		CurrentWeapon = Weapons[2];
	case ENewWeaponType::E_AXE:
		CurrentWeapon = Weapons[3];
		break;
	}

	if (WeaponOwner->GetCurrentPlayerState() == ENewPlayerState::E_BATTLE) {
		Draw();
	}

}
ENewWeaponType UMyNewWeaponManager::GetNextWeapon() {
	switch (CurrentWeaponType) {
	case ENewWeaponType::E_DUAL:
		return  ENewWeaponType::E_KATANA;
	case ENewWeaponType::E_KATANA:
		return  ENewWeaponType::E_AXE;
	case ENewWeaponType::E_AXE:
		return  ENewWeaponType::E_DUAL;
	default:
		return  ENewWeaponType::E_DUAL;
	}
}

AMyNewBaseWeapon* UMyNewWeaponManager::GetAttackWeapon(EWeaponHand hand) {
	switch (CurrentWeaponType) {
	case ENewWeaponType::E_DUAL:
		if (hand == EWeaponHand::E_LEFT)
			return Weapons[0];
		else
			return Weapons[1];
	case ENewWeaponType::E_KATANA:
		return Weapons[2];
	case ENewWeaponType::E_AXE:
		return Weapons[3];
	default:
		return nullptr;
	}
}