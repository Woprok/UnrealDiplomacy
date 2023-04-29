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
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10031;
	static const int32 RequiredParametersCount = 1;
	static FUDDealPointData ConvertData(FUDActionData& action)
	{
		return FUDDealPointData(action.ValueParameters);
	}
};