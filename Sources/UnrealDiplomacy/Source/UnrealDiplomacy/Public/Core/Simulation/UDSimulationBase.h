// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDSimulationBase.generated.h"

USTRUCT()
struct UNREALDIPLOMACY_API FUDSimulationInfo 
{
	GENERATED_BODY()
};

USTRUCT()
struct UNREALDIPLOMACY_API FUDSimulationOverview 
{
	GENERATED_BODY()
public:
	int32 Width;
	int32 Height;
	int32 Seed;
	int32 Step;
	TArray<FUDSimulationPlayer> Players;
};

USTRUCT()
struct UNREALDIPLOMACY_API FUDSimulationEnvironment 
{
	GENERATED_BODY()
public:
	int32 Width;
	int32 Height;
	int32 Seed;
};

USTRUCT()
struct UNREALDIPLOMACY_API FUDSimulationPlayer 
{
	GENERATED_BODY()
public:
	int32 Id;
};

USTRUCT()
struct UNREALDIPLOMACY_API FUDSimulationStep
{
	GENERATED_BODY()
public:
	int32 Step;
};

/**
 * Initialize
 * - define simulation environment
 * - define simulated objects
 * -- player, his existance in world, everything owned by existance
 * - define simulation step (what it is and how it changes)
 * - define simulation step for each object (what is expected to happen on each step)
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSimulationBase : public AUDActor
{
	GENERATED_BODY()
public:
	void InitializeEnvironment(FUDSimulationEnvironment environment);
	void InitializePlayer(FUDSimulationPlayer player);
	void StartStep(FUDSimulationStep step);
	void EndStep(FUDSimulationStep step);
protected:
	FUDSimulationOverview overview;
};
