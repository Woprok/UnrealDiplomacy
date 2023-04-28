// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "UDSystemActionPlayerRemove.generated.h"

/**
 * Removes state of existing player.
 */
UCLASS(Deprecated, Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API __declspec(deprecated) UUDSystemActionPlayerRemove : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 3;
};

/**
 * Ignores state of existing player.
 */
UCLASS(Deprecated, Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API __declspec(deprecated) UUDSystemActionPlayerIgnore : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 4;
};