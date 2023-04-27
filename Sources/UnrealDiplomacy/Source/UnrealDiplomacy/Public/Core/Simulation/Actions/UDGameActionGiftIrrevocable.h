// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionGiftIrrevocable.generated.h"

/**
 * Simple gift of resources between two players, without any strings attached.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionGiftIrrevocable : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1000;
	static const int32 RequiredParametersCount = 2;
	static FUDTargetValueData ConvertData(FUDActionData& data)
	{
		return FUDTargetValueData(data.ValueParameters);
	}
};