// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

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

	bIsInputEnabled = true;
}


// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(GEngine)
	{
	    CurrentVelocity = GetVelocity().Length();
		
		GEngine->AddOnScreenDebugMessage(1,
		INDEFINITELY_LOOPING_DURATION, 
		FColor::Green,
		FString::Printf(TEXT("Current Velocity: %f"), CurrentVelocity));
	}
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
	if (bIsInputEnabled)
	{
		if (bool CurrentValue = Value.Get<bool>())
		{
			const FVector WorldImpulseVector = FVector(0.f, 0.f, 1.f) * ImpulseStrength;
			const FVector LocalImpulse = GetActorRotation().RotateVector(WorldImpulseVector);
			ShipMesh->AddImpulse(LocalImpulse, NAME_None, true);
		}
	}
}

// Rotate  FUNCTION()
void AShip::Rotate(const FInputActionValue& Value)
{
	if (bIsInputEnabled)
	{
		if (float CurrentValue = Value.Get<float>())
		{
			const FVector Torque = FVector(1.f, 0.f, 0.f) * TorqueStrength * CurrentValue;
			ShipMesh->AddTorqueInRadians(Torque, NAME_None, true);
		}
	}
}

void AShip::NotifyHit(UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector HitImpulse,
	const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, HitImpulse, Hit);

    HandleShipLanding();
	
}

// Function to test if Actor (Ship) has landed safely
bool AShip::IsLandedSafely()
{
	FRotator CurrentRotation = GetActorRotation();

	float AcceptableRollRange = 80.f;

	return FMath::Abs(CurrentRotation.Roll) <= AcceptableRollRange;
}

void AShip::HandleShipLanding()
{
	if(!IsLandedSafely() || CurrentVelocity > MaxLandingVelocity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ship has crashed!"));
		
		// Reload current level if ship crashes
		FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
		   // or FName(*GetWorld()->GetName())
		UGameplayStatics::OpenLevel(this, CurrentLevelName, true);
	}
	else
	{
		// UE_LOG(LogTemp, Warning , TEXT("Ship has landed safely!"));
	}
}

void AShip::IsGoalReached()
{
	bIsInputEnabled = false;
}