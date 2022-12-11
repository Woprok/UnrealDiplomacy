// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "Skirmish/UDSkirmishGameMode.h"
#include "Skirmish/UDSkirmishAIController.h"
#include "Skirmish/UDSkirmishPlayerController.h"
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
};