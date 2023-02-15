// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDAIController.h"
#include "Core/UDControllerInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
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
	virtual void OnActionExecuted(FUDActionData executedAction) override;
	virtual void SetSimulatedStateAccess(TObjectPtr<UUDWorldState> personalWorldState) override
	{
		PersonalState = personalWorldState;
		GetPersonalAdministrator()->SetOverseeingState(PersonalState);
	}
	UFUNCTION()
	virtual void SetControllerUniqueId(int32 uniqueControllerId) override
	{
		UniqueControllerId = uniqueControllerId;
	}
	UFUNCTION()
	virtual int32 GetControllerUniqueId() override
	{
		return UniqueControllerId;
	}
	UFUNCTION()
	void InitializeAdministrator();
protected:
	TObjectPtr<UUDActionAdministrator> GetPersonalAdministrator()
	{
		if (!IsValid(InternalPersonalAdministrator))
		{
			InitializeAdministrator();
		}
		return InternalPersonalAdministrator;
	}
protected:
	UPROPERTY()
	int32 UniqueControllerId;
	UPROPERTY()
	TObjectPtr<UUDWorldState> PersonalState = nullptr;
	/**
	 * Handles logic verfication and game rules access over the state owned by this controller.
	 */
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> InternalPersonalAdministrator = nullptr;
private:
	void DoTurn();
	bool IsPlaying = false;
	TArray<int32> gifters;
};