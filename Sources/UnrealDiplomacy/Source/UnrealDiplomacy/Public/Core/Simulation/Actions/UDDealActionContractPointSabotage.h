// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionContractPointSabotage.generated.h"

/**
 * Sabotage the action.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionContractPointSabotage : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10050;
	static const int32 RequiredParametersCount = 2;
	static FUDDealValueData ConvertData(FUDActionData& data)
	{
		return FUDDealValueData(data.ValueParameters);
	}
};
