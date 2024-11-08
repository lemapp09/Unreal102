// Fill out your copyright notice in the Description page of Project Settings.

#include "LanderGameMode.h"
#include "GameFramework/GameUserSettings.h"
#include "Ship.h"
#include "Data/LevelTimerData.h"
#include "Kismet/GameplayStatics.h"

ALanderGameMode::ALanderGameMode()
{
    DefaultPawnClass = AShip::StaticClass();
	
	PrimaryActorTick.bCanEverTick = true;
	// CountdownTimer = 30.f;

	static ConstructorHelpers::FObjectFinder<UDataTable>
	   DataTableAsset(TEXT("/Script/Engine.DataTable'/Game/DataTables/LevelTimerData.LevelTimerData'"));

	if(DataTableAsset.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("DT Loaded Successfully"));
		LevelTimerDataTable = DataTableAsset.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Did not find Level Timer Data Table!"));
	}
}

void ALanderGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Get Game User Settings
	UGameUserSettings* GameUserSettings = Cast<UGameUserSettings>(UGameUserSettings::GetGameUserSettings());

	if (GameUserSettings)
	{
		// Load Settings
		GameUserSettings->LoadSettings(false);
	}

	
	if(LevelTimerDataTable != nullptr)
	{
		FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(this);

		for(auto It : LevelTimerDataTable->GetRowMap())
		{
			const FLevelTimerData* Row = reinterpret_cast<const FLevelTimerData*>(It.Value);
			if(Row->LevelName == CurrentLevelName)
			{
				CountdownTimer = Row->TimerValue;
			}
		}
	}
	
	Timer = CountdownTimer;

	APlayerController* PlayerController =
		Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, true));
	if(PlayerController)
	{
		FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
		if(CurrentLevelName == "MainMenu")
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
		else
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void ALanderGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(bIsTimeRunning)
	{
		if(Timer > 0)
	    {
		  Timer -= DeltaTime;
		
		  if(Timer <= 0)
		    {
			  // Restart the level
			  FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
			  UGameplayStatics::OpenLevel(this, CurrentLevelName);
		    }
	    }
	}
	
}

void ALanderGameMode::StopTimer()
{
	// Called by Goal Actor to stop the countdown timer
	bIsTimeRunning = false;
}

void ALanderGameMode::IncreaseTimer(float AdditionalTime)
{
	Timer += AdditionalTime;
}

void ALanderGameMode::LoadNextLevel()
{
	// Get the Current Level ID
	int32 CurrentLevelID = GetCurrentLevelID();

	// Check that we have received a Level ID
	if(CurrentLevelID != -1)
	{
		//  Iterate the Level ID to the next level
		int32 NextLevelID = CurrentLevelID + 1;

		// Once we have the level id, get level name of next level
		for  (auto It : LevelTimerDataTable->GetRowMap())
		{
			const FLevelTimerData* Row = reinterpret_cast<const FLevelTimerData*>(It.Value);
			if(Row && Row->LevelID == NextLevelID)
			{
					// Load the next level using founded level name
				UGameplayStatics::OpenLevel(this, Row->LevelName);
				return;
			}
		}
	}
}

int32 ALanderGameMode::GetCurrentLevelID() const
{
	FName CurrentLevelName = *UGameplayStatics::GetCurrentLevelName(this, true);
	for (auto It : LevelTimerDataTable->GetRowMap())
	{
		const FLevelTimerData* Row = reinterpret_cast<const FLevelTimerData*>(It.Value);
		if(Row && Row->LevelName == CurrentLevelName)
		{
			return Row->LevelID;
		}
	}
	return -1;
}
