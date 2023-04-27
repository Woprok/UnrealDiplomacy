// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionCreate.generated.h"

/**
 * Deal actions between any amount of players.
 * Core part of the game. Uses 10000-19999 for all subsequent actions.
 * Creates new deal.
 * Player that created the deal is considered a participant immediately.
 * Deals do not have an owner and are stored as part of world state.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionCreate : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10000;
	static const int32 RequiredParametersCount = 0;
};