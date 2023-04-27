// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionParticipantKick.generated.h"

/**
 * Leave deal and loose participation, same as rejecting, but can be done at any point, if player is fed up with it.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionParticipantKick : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10004;
	static const int32 RequiredParametersCount = 2;
	static FUDDealTargetData ConvertData(FUDActionData& data)
	{
		return FUDDealTargetData(data.ValueParameters);
	}
};