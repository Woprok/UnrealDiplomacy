// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.h"
#include "UDActionInterface.generated.h"

// TODO extend by adding more functions that do verify & check restriction

/** 
 * Specifier for interface used by IUDAction.
 * This is required by UE compiler. This can't contain UNREALDIPLOMACY_API macro.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUDActionInterface : public UInterface
{
	GENERATED_BODY()
};

/** 
 * Interface for executable action.
 * Action is defined by ability to take data and apply their expected effect on simulation.
 */
class UNREALDIPLOMACY_API IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData actionData);
	virtual void Revert(FUDActionData actionData);
	virtual int32 GetActionTypeId();
};

/**
 * Simple action example for debugging purposes.
 */ 
UCLASS()
class UNREALDIPLOMACY_API UUDLogAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData actionData) override;
	virtual void Revert(FUDActionData actionData) override;
	virtual int32 GetActionTypeId() override;
private:
	const int32 ActionTypeId = 0;
};