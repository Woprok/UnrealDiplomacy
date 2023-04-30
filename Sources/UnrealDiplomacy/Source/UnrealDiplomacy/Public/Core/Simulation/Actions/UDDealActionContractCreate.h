// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealAction.h"
#include "UDDealActionContractCreate.generated.h"

// Forward Declarations
struct FUDDiscussionAction;

/**
 * This action transforms preview of actions to final actions.
 * After this players can choose if they honour this deal.
 * This is executed on clients as well. The order is strictly determined for all actions.
 * for each invoker in ascending order, then for each target in ascending order.
 * The order must remain same as the actions are temporatily assigned array index as their Id.
 * Actions can't receive Id unless they are being executed and this would also mean that
 * simulation has to receive all actions from server. Which might be taxing!
 * Single item can easily result in exponential amount of actions.
 * TODO change this to receive from server might be in the end safer for sync.
 * Current version does not support partial information about the result.
 * In theory there is no way to have player believe that something did happen.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionContractCreate : public UUDDealAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealData::ParameterCount; };
public:
	static const int32 ActionTypeId = 3018;
	static TArray<FUDActionData> FinalizeActions(TObjectPtr<UUDWorldState> world, int32 dealUniqueId);
	static TArray<FUDDiscussionAction> WrapActions(TArray<FUDActionData> data);
};