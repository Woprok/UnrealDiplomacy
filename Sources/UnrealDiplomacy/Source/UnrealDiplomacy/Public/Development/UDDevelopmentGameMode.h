// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "Skirmish/UDSkirmishGameMode.h"
#include "Skirmish/UDSkirmishAIController.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "UDDevelopmentAIController.h"
#include "UDDevelopmentGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDDevelopmentGameMode : public AUDSkirmishGameMode
{
	GENERATED_BODY()
public:
	AUDDevelopmentGameMode();

	virtual void BeginPlay() override;
protected:

private:
	virtual TObjectPtr<AUDSkirmishAIController> CreateAi() override;
	// Primary server simulation.
	//TObjectPtr<AUDSimulationBase> Simulation;
	// Secondary simulations that are used for Ai Agents and verification/restoration of Human Agent state.
	//TMap<int32, TObjectPtr<AUDSimulationBase>> AgentsSimulations;
};
