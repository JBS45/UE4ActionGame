// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectComponent.h"
#include "Components/SphereComponent.h"
#include "../Monster/BaseMonster.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDetectComponent::UDetectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	DetectSphere = CreateDefaultSubobject<USphereComponent>("Detect");
	Range = 2500.0f;
	CollisonProfile = FName("DetectMonster");
	LockOnTarget = nullptr;
}


// Called when the game starts
void UDetectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentTargetsIndex = 0;
	SetDetectSphere();
}


// Called every frame
void UDetectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	CheckTargets();
	CheckLockOnTarget();
}

void UDetectComponent::SetDetectSphere()
{

	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &UDetectComponent::OnOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &UDetectComponent::OnOverlapEnd);

	DetectSphere->SetSphereRadius(Range);

	DetectSphere->SetCollisionProfileName(CollisonProfile);
	DetectSphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

}

void UDetectComponent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	auto target = Cast <ABaseMonster>(OtherActor);
	if (target != nullptr && target->GetIsAlive()) {
		Targets.Add(target);
		TESTLOG(Warning, TEXT("%s"), *target->GetName())
	}
}
void UDetectComponent::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	auto target = Cast <ABaseMonster>(OtherActor);
	if (target != nullptr) {
		if (Targets.Contains(target)) {
			Targets.Remove(target);
		}
	}
}
void UDetectComponent::LockOnCamera()
{
	if (Targets.Num() <= 0) return;

	if (LockOnTarget == nullptr) {
		CurrentTargetsIndex = 0;
		if (Targets[CurrentTargetsIndex] == nullptr) return;
	}

	LockOnTarget = Targets[CurrentTargetsIndex];
	IsLockOn = true;
	Notify();

}
void UDetectComponent::LockOff()
{
	IsLockOn = false;
	Notify();
}
void UDetectComponent::SwitchingCamera()
{
	if (IsLockOn == false
		|| Targets.Num() <= 1) return;

	CurrentTargetsIndex++;
	if (CurrentTargetsIndex >= Targets.Num()) {
		CurrentTargetsIndex = 0;
	}
	LockOnTarget = Targets[CurrentTargetsIndex];
	Notify();
}
void UDetectComponent::CheckTargets() {
	if (Targets.Num() <= 0) return;

	for (int i = 0; i < Targets.Num(); ++i) {
		if (Targets[i]->GetIsAlive() == false) {
			Targets.RemoveAt(i);
			i--;
		}
	}
}

void UDetectComponent::CheckLockOnTarget() {
	if (IsLockOn) {
		if (!Targets.Contains(LockOnTarget)) {
			IsLockOn = false;
			LockOnTarget = nullptr;
			Notify();
		}
	}
}
ABaseMonster* UDetectComponent::GetLockTarget() {
	return LockOnTarget;
}

void UDetectComponent::Attach(ICameraLockOnInterface* obserever) {
	LockOnObservers.Add(obserever);
}

void UDetectComponent::Notify() {
	for (auto observer : LockOnObservers) {
		observer->NotifyLockOnData(IsLockOn, LockOnTarget);
	}
}