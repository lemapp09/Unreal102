// Fill out your copyright notice in the Description page of Project Settings.

// TODO: Notify the Goal that it has been hit (Physics Hit)
// TODO: By the ship actor
// TODO: Stop Timer?? Need a Timer
// TODO: Activate Celebration

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

UCLASS()
class UNREAL_102_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit) override;
	
	void HandleGoalReached();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Bool to track when Goal has been hit
	UPROPERTY(VisibleAnywhere, Category="Ship | Goal")
	bool bIsGoalReached = false;

};