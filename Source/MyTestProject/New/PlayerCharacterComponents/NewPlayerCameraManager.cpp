// Fill out your copyright notice in the Description page of Project Settings.


#include "NewPlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "../PlayerCharacter/MyNewPlayerController.h"
#include "../NewMonster/MyNewBaseMonster.h"
#include "Kismet/KismetMathLibrary.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../PlayerCharacter/MyNewPlayerController.h"

ANewPlayerCameraManager::ANewPlayerCameraManager() {
	ViewPitchMin = -60;
	ViewPitchMax = 10;
	LockOnPitchMax = -10;
}

void ANewPlayerCameraManager::SetCamera(UCameraComponent* camera, USpringArmComponent* Arm, AMyNewCharacter* Player, UNewCameraDetect* Detect, AMyNewPlayerController* control)
{
	MainCamera = camera;
	MainArm = Arm;

	CurrentPlayer = Player;
	Detector = Detect;
	Controller = control;

	IsLockOn = false;
	TargetIndex = 0;
}
void ANewPlayerCameraManager::ShakeCamera(float scale){
	PlayCameraShake(UNewCameraShake::StaticClass(), scale);
}
void ANewPlayerCameraManager::ToggleLockOn() {
	if (IsLockOn == false) {
		IsLockOn = true;
	}
	else {
		IsLockOn = false;
	}
}

AMyNewBaseMonster* ANewPlayerCameraManager::CameraLockOn(float delta) {
	if (IsLockOn && Detector->GetTargets().Num()>0) {
		AMyNewBaseMonster* Target = Detector->GetTargets()[TargetIndex];
		FRotator DestRotation = UKismetMathLibrary::FindLookAtRotation(CurrentPlayer->GetActorLocation(), Target->GetActorLocation());
		auto TempRot = FMath::RInterpTo(Controller->GetControlRotation(), DestRotation, delta, 10.0f);
		if (TempRot.Pitch < LockOnPitchMax) {
			Controller->SetControlRotation(TempRot);
		}
		else {
			TempRot.Pitch = LockOnPitchMax;
			Controller->SetControlRotation(TempRot);
		}

		//PlayerHUD->TraceTarget(this, Target);

		float Distance = FVector::Distance(CurrentPlayer->GetActorLocation(), Target->GetActorLocation());
		if (Distance > Detector->GetDetectRange() || Target->IsAlive()==false) {
			IsLockOn = false;
			TargetIndex = 0;
			return nullptr;
		}
		return Target;
	}
	return nullptr;
}
void ANewPlayerCameraManager::ChangeLockOn() {
	NextTarget(TargetIndex);
}

bool ANewPlayerCameraManager::NextTarget(int32& index) {
	if (!IsLockOn&&Detector->GetTargets().Num() < 0) {
		return false;
	}
	index++;
	if (index >= Detector->GetTargets().Num()) {
		index = 0;
	}
	return true;
}

UNewCameraShake::UNewCameraShake() {
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(25.0f, 35.0f);

	RotOscillation.Yaw.Amplitude = FMath::RandRange(5.0f, 10.0f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(25.0f, 35.0f);
}

UNewCameraDetect::UNewCameraDetect() {
	PrimaryComponentTick.bCanEverTick = true;

	Range = CreateDefaultSubobject<USphereComponent>("Range");
	Range->SetCollisionProfileName("DetectMonster");
}
void UNewCameraDetect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckTarget();
}

void UNewCameraDetect::Init(float range,USkeletalMeshComponent* Mesh) {
	Range->SetSphereRadius(range);
	Range->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	Range->OnComponentBeginOverlap.AddDynamic(this, &UNewCameraDetect::OnOverlapBegin);
	Range->OnComponentEndOverlap.AddDynamic(this, &UNewCameraDetect::OnOverlapEnd);
}

void UNewCameraDetect::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	auto Monster = Cast<AMyNewBaseMonster>(OtherActor);
	if (IsValid(Monster) && Monster->IsAlive()) {
		Targets.Add(Monster);
	}
}

void UNewCameraDetect::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	int32 index;
	bool result = Targets.Find(Cast<AMyNewBaseMonster>(OtherActor),index);
	if (result) {
		Targets.RemoveAt(index);
	}
}

void UNewCameraDetect::CheckTarget() {
	for (int i = 0; i < Targets.Num(); ++i) {
		if (!Targets[i]->IsAlive()) {
			Targets.RemoveAt(i);
			i--;
		}
	}
}