// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSystemActionLog.generated.h"

/**
 * Simple action example for debugging purposes.
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDSystemActionLog : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 0;
	static const int32 RequiredParametersCount = 0;
};