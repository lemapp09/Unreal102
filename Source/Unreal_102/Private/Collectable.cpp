// Fill out your copyright notice in the Description page of Project Settings.

#include "Collectable.h"
#include "Ship.h"
#include "LanderGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollectableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectableMesh"));
	SetRootComponent(CollectableMesh);

	CollectableMesh->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnOverlapBegin);
	// Collision Profile
	CollectableMesh->SetCollisionProfileName("OverlapAllDynamic");
}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectable::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != this && OtherActor->IsA(AShip::StaticClass()))
	{
		Collect();
	}
}

void ACollectable::Collect()
{
	// Increase Timer
	if(ALanderGameMode* LanderGameMode = Cast<ALanderGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		LanderGameMode->IncreaseTimer(CollectableValue);
	}

	// Destory Collectable
	this->Destroy();
}