// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionGift.generated.h"

/**
 * Simple gift of resources between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionGift : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1001;
	static const int32 RequiredParametersCount = 2;
	static FUDTargetValueData ConvertData(FUDActionData& data)
	{
		return FUDTargetValueData(data.ValueParameters);
	}
};