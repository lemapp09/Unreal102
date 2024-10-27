// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SkeletalMeshAttributes.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	SetRootComponent(ShipMesh);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(ShipMesh);
	CameraBoom->TargetArmLength = 1000.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ShipMappingContext, 0);
		}
			
	}
	
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind our Actions to a Function
		EnhancedInputComponent->BindAction(PropelUpAction, ETriggerEvent::Triggered, this, &AShip::PropelUp);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AShip::Rotate);
	}
}

// PropelUp  FUNCTION()
void AShip::PropelUp(const FInputActionValue& Value)
{ 
	if (bool CurrentValue = Value.Get<bool>())
	{
		const FVector WorldImpulseVector = FVector(0.f, 0.f, 1.f) * ImpulseStrength;
		const FVector LocalImpulse = GetActorRotation().RotateVector(WorldImpulseVector);
		ShipMesh->AddImpulse(LocalImpulse, NAME_None, true);
		// ShipMesh->AddForceAtLocationLocal(WorldImpulseVector, LocalImpulse, NAME_None);
	}
}

// Rotate  FUNCTION()
void AShip::Rotate(const FInputActionValue& Value)
{
	if (float CurrentValue = Value.Get<float>())
	{
		UE_LOG(LogTemp, Warning, TEXT("IA_Rotate triggered! %f"), CurrentValue);
		const FVector Torque = FVector(1.f, 0.f, 0.f) * TorqueStrength * CurrentValue;
		ShipMesh->AddTorqueInRadians(Torque, NAME_None, true);
	}
}
