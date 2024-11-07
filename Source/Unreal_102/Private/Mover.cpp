// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MovementSpeed = 100.f;
	bMovingToEnd = true;
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	if(AActor* Owner = GetOwner())
	{
		USceneComponent* ActorRoot = Owner->GetRootComponent();
		ActorRoot->SetMobility(EComponentMobility::Movable);
		WorldStartPosition = Owner->GetTransform().TransformPosition(LocalStartPosition);
		WorldEndPosition = Owner->GetTransform().TransformPosition(LocalEndPosition);
	}
	
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!GetOwner())return;

	FVector CurrentPosition = GetOwner()->GetActorLocation();
	FVector TargetPosition = bMovingToEnd ? WorldEndPosition : WorldStartPosition;
	FVector Direction = (TargetPosition - CurrentPosition).GetSafeNormal();
	FVector NewPosition = CurrentPosition + MovementSpeed * DeltaTime * Direction;

	if(FVector:: Dist(NewPosition, TargetPosition) < FVector:: Dist(CurrentPosition, TargetPosition))
	{
		GetOwner()->SetActorLocation(NewPosition);
	}
	else
	{
		GetOwner()->SetActorLocation(TargetPosition);
		bMovingToEnd = !bMovingToEnd;
	}
}

