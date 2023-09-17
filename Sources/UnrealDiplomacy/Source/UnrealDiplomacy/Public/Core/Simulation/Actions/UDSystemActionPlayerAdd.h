// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "UDSystemActionPlayerAdd.generated.h"

/**
 * Defines state for new player.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSystemActionPlayerAdd : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 12;
};