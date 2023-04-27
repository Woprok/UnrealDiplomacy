// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSystemActionInvalid.generated.h"

/**
 * Handles action that are passed to execution with undefined executor.
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDSystemActionInvalid : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	//virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	//virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = -1;
	static const int32 RequiredParametersCount = 0;
};