// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterIKComponent.generated.h"

class ABaseCharacter;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYTESTPROJECT_API UCharacterIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterIKComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:

	bool IsActive = false;
	ABaseCharacter* Character;
	float CapusuleHeight;

	FName LeftSocketName;
	FName RightSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float IKFootOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float IKTraceDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float IKFootInterepSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float IKCapuseInterpSpeed;

	FRotator LeftRotation;
	FRotator RightRotation;

	float HipOffset;

	float RightValue;
	float LeftValue;

	float Displacement;
private:
	void UpdateIK(float delta);
	void FootTrace(FName socketName, float &offset, FVector& Normal, FHitResult& Hit);
	FRotator NormalToRotator(FVector impactPoint);
	void LerpRotation(float delta, FRotator rotation, FRotator& footRotation, float InterpSpeed);
	void FootOffset(float delta, float target, float& effector, float speed);
	void UpdateCapsule(float delta, float Hipshift, bool reset);


	/*
	void LineTrace(FName socketName, float& dist, FHitResult& Hit, FVector& Normal);
	FRotator MakeIK(FName socketName, float& IKValue, float delta, float speed);
	void LerpDisplacement(float left, float right, float delta, float speed);*/
public:
	FORCEINLINE FRotator GetLeftRotation();
	FORCEINLINE FRotator GetRIghtRotation();
	FORCEINLINE float GetLeftIKValue();
	FORCEINLINE float GetRightIKValue();
	FORCEINLINE float GetHipOffset();
};