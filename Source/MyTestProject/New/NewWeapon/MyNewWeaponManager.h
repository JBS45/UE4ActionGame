// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../PlayerCharacterInterface/MyNewPlayerInterface.h"
#include "MyNewWeaponManager.generated.h"


class AMyNewCharacter;
class AMyNewBaseWeapon;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTESTPROJECT_API UMyNewWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyNewWeaponManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Onwer", meta = (AllowPrivateAccess = "true"))
		AMyNewCharacter* WeaponOwner;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		TArray<AMyNewBaseWeapon*> Weapons;
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		ENewWeaponType CurrentWeaponType;
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		AMyNewBaseWeapon* CurrentWeapon;


	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FName AttachLeftPutUpSocket;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FName AttachRightPutUpSocket;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FName AttachSwordPutUpSocket;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FName AttachAxePutUpSocket;

	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FName AttachLeftDrawSocket;
	UPROPERTY(EditAnywhere, Category = "Socket", meta = (AllowPrivateAccess = "true"))
		FName AttachRightDrawSocket;

	bool IsInit;
	
public:
	void ChangeWeaponState(const ENewWeaponType weapon);
	void SetInit(AMyNewCharacter* owner);
	void Draw();
	void PutUp();
	void PlayerDead();
	ENewWeaponType GetNextWeapon();
	AMyNewBaseWeapon* GetAttackWeapon(EWeaponHand hand);
public:
	FORCEINLINE AMyNewBaseWeapon* GetCurrentWeapon() { return CurrentWeapon; };
};
