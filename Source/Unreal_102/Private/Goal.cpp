// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "LanderGameMode.h"
#include "NiagaraFunctionLibrary.h"
#include "Ship.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsWaitingForParticleFX)
	{
		if(CelebrationEffectComponent && !CelebrationEffectComponent->IsActive())
		{
	        ALanderGameMode* LanderGameMode =
				Cast<ALanderGameMode>(UGameplayStatics::GetGameMode(this));

			if(LanderGameMode)
			{
				LanderGameMode->LoadNextLevel();
			}
		}
	}
}

void AGoal::NotifyHit(UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Functionality of the Hit Event
	if (!bIsGoalReached)
	{
		if(Other && Other != this && Other->IsA(AShip::StaticClass()))
		{
			HandleGoalReached();

			AShip* ShipActor = Cast<AShip>(Other);
			if (ShipActor)
			{
				ShipActor->IsGoalReached();
			}
		}
	}
}
void AGoal::HandleGoalReached() 
{
	bIsGoalReached = true;

	if(CelebrationEffect)
	{
		FVector EffectLocation = GetActorLocation();
		CelebrationEffectComponent =
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			CelebrationEffect, EffectLocation, GetActorRotation());
		
		if(CelebrationEffectComponent)
			{
			bIsWaitingForParticleFX = true;
	
			ALanderGameMode* LanderGameMode =
				Cast<ALanderGameMode>(UGameplayStatics::GetGameMode(this));

			if(LanderGameMode)
				{
					LanderGameMode->StopTimer();

					// Need to wait for Effect to finish before loading next level
					//LanderGameMode->LoadNextLevel();
				}
			}
	}
}