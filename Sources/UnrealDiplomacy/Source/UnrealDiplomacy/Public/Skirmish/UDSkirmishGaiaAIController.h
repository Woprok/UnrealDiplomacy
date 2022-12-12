// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/UDAIController.h"
#include "Core/UDControllerInterface.h"
#include "UDSkirmishGaiaAIController.generated.h"

/**
 * Generic version of an controller, that is restricted to server.
 * Does not provide complex decision making, instead is providing rules to
 * the world and handles all neutral action invocations. 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishGaiaAIController : public AUDAIController, public IUDControllerInterface, public IUDActionHandlingInterface
{
	GENERATED_BODY()
public:
	virtual void OnActionExecuted(FUDActionData& executedAction) override;
	virtual void SetSimulatedStateAccess(TObjectPtr<UUDWorldState> personalWorldState) override
	{
		GaiaState = personalWorldState;
	}
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override
	{
		UniqueControllerId = uniqueControllerId;
	}
	virtual int32 GetControllerUniqueId() override
	{
		return UniqueControllerId;
	}
protected:
	UPROPERTY()
	int32 UniqueControllerId;
	UPROPERTY()
	TObjectPtr<UUDWorldState> GaiaState = nullptr;
};