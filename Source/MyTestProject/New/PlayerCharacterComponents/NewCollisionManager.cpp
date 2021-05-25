// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCollisionManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UNewCollisionManager::UNewCollisionManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	BlockRadius = 34.0f;
	BlockHeight = 80.0f;
	LerpSpeed = 600.0f;
	BlockChannel = "BoundingBox";
	CanBlocking = false;
}

void UNewCollisionManager::Init() {
	CanBlocking = false;
	Character = Cast<ACharacter>(GetOwner());
	CharacterMovement = Character->GetCharacterMovement();
}
// Called every frame
void UNewCollisionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (CanBlocking) {
		BlockOtherActor(DeltaTime);
	}
}

void UNewCollisionManager::BlockOtherActor(float delta) {

	FVector Location = Character->GetActorLocation();

	TArray<AActor*> Ignore;
	FHitResult HitResult;
	Ignore.Add(Character);
	bool Result = UKismetSystemLibrary::CapsuleTraceSingleByProfile(GetWorld(), Location, Location,
		BlockRadius, BlockHeight, BlockChannel, true, Ignore, EDrawDebugTrace::ForOneFrame, HitResult, true, FLinearColor::Blue, FLinearColor::Green, 1.0f);
	if (Result) {

		FVector Direction = HitResult.GetActor()->GetActorLocation()- Location;

		float flag = 1.0f;

		float Dot = UKismetMathLibrary::Dot_VectorVector(Direction, HitResult.ImpactNormal);
		if (Dot >= 0) {
			flag = -1.0f;
		}
		else {
			flag = 1.0f;
		}

		Direction = FVector(Direction.X, Direction.Y, 0);
		FVector ImpactPoint = HitResult.ImpactPoint;

		FVector Start = Location;
		FVector End = FVector(ImpactPoint.X, ImpactPoint.Y, Location.Z);

		float OverlapValue = FVector::Distance(Start, End);

		FVector Dir = HitResult.ImpactNormal;
		Dir = FVector(Dir.X, Dir.Y, 0);
		Dir.Normalize();

		FVector Target = Location - ((Dir*flag)*(OverlapValue - BlockRadius));
		Character->SetActorLocation(Target);
	}
}

void UNewCollisionManager::BlockActive(bool IsOn) {
	CanBlocking = IsOn;
}