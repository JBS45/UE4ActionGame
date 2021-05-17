// Fill out your copyright notice in the Description page of Project Settings.



#include "CharacterIKComponent.h"
#include "../Player/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCharacterIKComponent::UCharacterIKComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	LeftSocketName = FName("foot_l");
	RightSocketName = FName("foot_r");

	IKFootOffset = 5.0f;
	IKTraceDistance = 60.0f;
	IKFootInterepSpeed = 20.0f;
	IKCapuseInterpSpeed = 20.0f;
}


// Called when the game starts
void UCharacterIKComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Character = Cast<ABaseCharacter>(GetOwner());
	CapusuleHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();


}

void UCharacterIKComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

// Called every frame
void UCharacterIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsActive) return;

	UpdateIK(DeltaTime);

}

void UCharacterIKComponent::UpdateIK(float delta)
{
	float LeftOffset = 0;
	float RightOffset = 0;
	FHitResult LeftHit;
	FHitResult RightHit;
	FVector LeftNormal;
	FVector RightNormal;

	FootTrace(LeftSocketName, LeftOffset, LeftNormal, LeftHit);
	FootTrace(RightSocketName, RightOffset, RightNormal, RightHit);

	LerpRotation(delta, NormalToRotator(LeftNormal), LeftRotation, IKFootInterepSpeed);
	LerpRotation(delta, NormalToRotator(RightNormal), RightRotation, IKFootInterepSpeed);

	float CapsuleOffset = UKismetMathLibrary::Min(LeftOffset, RightOffset);
	if (CapsuleOffset <= 0) CapsuleOffset = 0;


	FootOffset(delta, CapsuleOffset, HipOffset, IKFootInterepSpeed);
	UpdateCapsule(delta, CapsuleOffset, false);

	FootOffset(delta, LeftOffset - CapsuleOffset, LeftValue, IKFootInterepSpeed);
	FootOffset(delta, CapsuleOffset - RightOffset, RightValue, IKFootInterepSpeed);
}

void UCharacterIKComponent::FootTrace(FName socketName, float& offset, FVector& Normal, FHitResult& Hit)
{

	FVector SocketLocation = Character->GetMesh()->GetSocketLocation(socketName);

	FVector Start = FVector(SocketLocation.X, SocketLocation.Y, Character->GetActorLocation().Z);
	FVector End = FVector(SocketLocation.X, SocketLocation.Y
		, (Character->GetActorLocation().Z - CapusuleHeight - IKTraceDistance));


	FHitResult HitResult;
	TArray<AActor*> Ignore;
	Ignore.Add(Character);


	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), true, Ignore, EDrawDebugTrace::ForOneFrame, HitResult, true, FLinearColor::Blue);

	Normal = HitResult.ImpactNormal;
	if (HitResult.IsValidBlockingHit()) {
		offset = IKFootOffset + ((HitResult.ImpactPoint - HitResult.TraceEnd).Size() - IKTraceDistance);
	}
	else {
		offset = 0.0f;
	}

	Hit = HitResult;
}


FRotator UCharacterIKComponent::NormalToRotator(FVector impactPoint)
{
	float FootRotX = UKismetMathLibrary::DegAtan2(impactPoint.Y, impactPoint.Z);
	float FootRotY = UKismetMathLibrary::DegAtan2(impactPoint.X, impactPoint.Z);
	FootRotY *= -1.0f;
	FRotator Rotation = FRotator(FootRotY, 0.0f, FootRotX);

	return Rotation;
}
void UCharacterIKComponent::LerpRotation(float delta, FRotator rotation, FRotator& footRotation, float InterpSpeed)
{
	//! Set Foot Rotation value with FInterpTo
	FRotator LerpRotator = UKismetMathLibrary::RInterpTo(footRotation, rotation, delta, InterpSpeed);
	footRotation = LerpRotator;
}
void UCharacterIKComponent::FootOffset(float delta, float target, float& effector, float speed) {
	float LerpValue = UKismetMathLibrary::FInterpTo(effector, target, delta, speed);
	effector = LerpValue;
}
void UCharacterIKComponent::UpdateCapsule(float delta, float Hipshift, bool reset) {
	UCapsuleComponent* capsule = Character->GetCapsuleComponent();

	float CapsuleHalf = 0.0f;
	if (reset)
	{
		CapsuleHalf = CapusuleHeight;
	}
	else
	{
		float HalfAbsSize = UKismetMathLibrary::Abs(Hipshift) * 0.5f;
		CapsuleHalf = CapusuleHeight - HalfAbsSize;
	}

	//! Set capsule component height with FInterpTo 
	float ScaledCapsuleHalfHeight = capsule->GetScaledCapsuleHalfHeight();
	float InterpValue = UKismetMathLibrary::FInterpTo(ScaledCapsuleHalfHeight, CapsuleHalf, delta, IKCapuseInterpSpeed);

	capsule->SetCapsuleHalfHeight(InterpValue, true);
}

FRotator UCharacterIKComponent::GetLeftRotation()
{
	return LeftRotation;
}
FRotator UCharacterIKComponent::GetRIghtRotation()
{
	return RightRotation;
}

float UCharacterIKComponent::GetLeftIKValue()
{
	return LeftValue;
}
float UCharacterIKComponent::GetRightIKValue()
{
	return RightValue;
}
float UCharacterIKComponent::GetHipOffset()
{
	return HipOffset;
}

