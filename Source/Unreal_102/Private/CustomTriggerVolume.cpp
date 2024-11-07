// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTriggerVolume.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UCustomTriggerVolume::UCustomTriggerVolume()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCustomTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	AActor* ParentActor = GetOwner();
	if(ParentActor)
	{
		UBoxComponent* BoxComponent = ParentActor->FindComponentByClass<UBoxComponent>();
		if(BoxComponent)
		{
		   BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UCustomTriggerVolume::OnOverlapBegin);
		}
	}
}

void UCustomTriggerVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetOwner())
	{
	   // TODO: Restart the Level
		UE_LOG(LogTemp, Error, TEXT("Out of Bounds!"));
	}
}


// Called every frame
void UCustomTriggerVolume::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
