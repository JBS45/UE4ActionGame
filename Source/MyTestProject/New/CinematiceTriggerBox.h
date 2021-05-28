// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CinematiceTriggerBox.generated.h"


class UBoxComponent;
class ULevelSequence;
class ULevelSequencePlayer;
class AMyNewCharacter;

UCLASS()
class MYTESTPROJECT_API ACinematiceTriggerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACinematiceTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	bool IsPlayCinematic;

	UPROPERTY(EditAnywhere, Category = "TriggerBox", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* TriggerBox;
	UPROPERTY(EditAnywhere, Category = "Sequence", meta = (AllowPrivateAccess = "true"))
		ULevelSequence* BossCinematic;

	AMyNewCharacter* Player;

	ULevelSequencePlayer* USequencePlayer;
public:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	void PlaySequence();
	UFUNCTION()
		void EndSequence();
};
