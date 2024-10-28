// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
 
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Ship.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class UNREAL_102_API AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector HitImpulse,
	const FHitResult& Hit) override;

	// Safe Landing Function
	bool IsLandedSafely();

	void HandleShipLanding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ship | Movement")
	float ImpulseStrength = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ship | Movement")
	float TorqueStrength = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ship | Movement")
	float MaxLandingVelocity = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ship | Graphics")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ship | Input")
	UInputMappingContext* ShipMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ship | Input")
	UInputAction* PropelUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ship | Input")
	UInputAction* RotateAction;
	
	void PropelUp(const FInputActionValue& Value);
	
	void Rotate(const FInputActionValue& Value);

private:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere);
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentVelocity;
};