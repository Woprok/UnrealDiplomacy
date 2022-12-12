// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDAIController.h"
#include "Core/UDControllerInterface.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "Core/Simulation/UDActionHandlingInterface.h"
#include "UDSkirmishAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishAIController : public AUDAIController, public IUDControllerInterface, public IUDActionHandlingInterface
{
	GENERATED_BODY()
public:
	virtual void OnActionExecuted(FUDActionData& executedAction) override;
	virtual void SetSimulatedStateAccess(TObjectPtr<UUDWorldState> personalWorldState) override
	{
		PersonalState = personalWorldState;
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
	TObjectPtr<UUDWorldState> PersonalState = nullptr;
};