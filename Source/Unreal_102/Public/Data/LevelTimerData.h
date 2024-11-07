#pragma once

# include "CoreGlobals.h"
# include "LevelTimerData.generated.h"

USTRUCT(BlueprintType)
struct FLevelTimerData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimerValue;
	
};
