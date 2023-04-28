// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionParticipantLeave.generated.h"

/**
 * Leave deal and loose participation, same as rejecting, but can be done at any point, if player is fed up with it.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionParticipantLeave : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10004;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& action)
	{
		return FUDDealData(action.ValueParameters);
	}
};