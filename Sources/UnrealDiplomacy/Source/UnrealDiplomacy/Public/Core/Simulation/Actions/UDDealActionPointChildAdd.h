// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointChildAdd.generated.h"

/**
 * Allows extending of the deal with new discussion point.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointChildAdd : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10033;
	static const int32 RequiredParametersCount = 2;
	static FUDDealPointData ConvertData(FUDActionData& action)
	{
		return FUDDealPointData(action.ValueParameters);
	}
};