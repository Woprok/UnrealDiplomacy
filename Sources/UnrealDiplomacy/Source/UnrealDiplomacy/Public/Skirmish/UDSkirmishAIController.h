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
	/**
	 * Binds this interface to particular WorldSimulation authority.
	 */
	virtual void ListenActionExecutor(TObjectPtr<AUDWorldSimulation> authority);
	virtual void OnActionExecuted(FUDActionData& executedAction) override;

	virtual void SetControllerUniqueId(int32 uniqueControllerId) override;
	virtual int32 GetControllerUniqueId() override;
private:
	int32 UniqueControllerId;
	TObjectPtr<UUDWorldState> PersonalState;
};
