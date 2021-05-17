// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBlockComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCharacterBlockComponent::UCharacterBlockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	BlockRadius = 36.0f;
	BlockHeight = 88.0f;
	LerpSpeed = 600.0f;
	BlockChannel = "BoundingBox";
}


// Called when the game starts
void UCharacterBlockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CanBlocking = false;
	Character = Cast<ACharacter>(GetOwner());
	CharacterMovement = Character->GetCharacterMovement();
}


// Called every frame
void UCharacterBlockComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (CanBlocking) {
		BlockOtherActor(DeltaTime);
	}
}

void UCharacterBlockComponent::BlockOtherActor(float delta) {

	FVector Location = Character->GetActorLocation();

	TArray<AActor*> Ignore;
	FHitResult HitResult;
	Ignore.Add(Character);
	bool Result = UKismetSystemLibrary::CapsuleTraceSingleByProfile(GetWorld(), Location, Location,
		BlockRadius, BlockHeight, BlockChannel, true, Ignore, EDrawDebugTrace::ForOneFrame, HitResult, true, FLinearColor::Blue, FLinearColor::Green, 1.0f);
	if (Result) {

		FVector ImpactPoint = HitResult.ImpactPoint;

		FVector Start = Location;
		FVector End = FVector(ImpactPoint.X, ImpactPoint.Y, Location.Z);

		float OverlapValue = FVector::Distance(Start, End);

		FVector Dir = HitResult.ImpactNormal;
		Dir = FVector(Dir.X, Dir.Y, 0);
		Dir.Normalize();

		FVector Target = Location - (Dir*(OverlapValue - BlockRadius));
		Character->SetActorLocation(Target);
	}
}

void UCharacterBlockComponent::BlockActive(bool IsOn) {
	CanBlocking = IsOn;
}