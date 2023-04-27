// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointIgnore.generated.h"

/**
 * Removes discussion point from the deal. Item is still stored and ready to be used, if revert is called.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointIgnore : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10031;
	static const int32 RequiredParametersCount = 1;
	static FUDDealPointData ConvertData(FUDActionData& data)
	{
		return FUDDealPointData(data.ValueParameters);
	}
};