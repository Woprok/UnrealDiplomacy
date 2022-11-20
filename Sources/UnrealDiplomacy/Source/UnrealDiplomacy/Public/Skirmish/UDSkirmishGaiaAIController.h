// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
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
	/**
	 * TODO
	 * Temporary enable of ticks, should be changed to event handler.
	 */
	//AUDSkirmishGaiaAIController();
	/**
	 * Every decision is done via ticks.
	 */
	//void Tick(float DeltaSeconds) override;
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	virtual int32 GetControllerUniqueId() override;
private:
	int32 UniqueControllerId;
};