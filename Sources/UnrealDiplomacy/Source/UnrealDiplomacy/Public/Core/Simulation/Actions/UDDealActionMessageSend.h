// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionMessageSend.generated.h"

/**
 * Updates discussion point from the deal.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionMessageSend : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10032;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& data)
	{
		return FUDDealData(data.ValueParameters);
	}
};
