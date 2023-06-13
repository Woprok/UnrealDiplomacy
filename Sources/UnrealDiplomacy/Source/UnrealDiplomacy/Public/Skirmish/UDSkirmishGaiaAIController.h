// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDAIController.h"
#include "UDSkirmishGaiaAIController.generated.h"

/**
 * Default controller for Gaia Faction.
 * Gaia Faction is controlled by server and handles server requests.
 * In other words this executes game rules and transitions.
 * Such as start of the turn cycle.
 * 
 * Basically server is not allowed to execute action and needs proxy.
 * This allows all actions made by anyone to be backtracked.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishGaiaAIController : public AUDAIController
{
	GENERATED_BODY()
public:
protected:
	// Action Execution
	virtual void ProcessIntermezzoPlay() override;
private:
	// AI Memory fields.
};