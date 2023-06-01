// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDAIController.h"
#include "UDSkirmishAIController.generated.h"

/**
 * Default controller for AI controlled factions.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishAIController : public AUDAIController
{
	GENERATED_BODY()
protected:
	// Action Execution
	virtual void ProcessOutTurnPlay() override;
	virtual void ProcessInTurnPlay() override;
private:
	void ResolveRequests();
private:
	// AI Memory fields.
	TArray<int32> gifters;
};