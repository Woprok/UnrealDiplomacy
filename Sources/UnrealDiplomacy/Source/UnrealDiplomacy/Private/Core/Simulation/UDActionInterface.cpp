// Copyright Miroslav Valach

#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDModifier.h"
#include "Core/Simulation/UDModifierManager.h"

// TODO LIST
// TODO extend by adding more functions that do verify & check restriction
// UUDUnconditionalGiftAction is missing condition (actionData.InvokerPlayerId != actionData.TargetPlayerId)
// TODO CanExecute overloads that check if the action is still queued in PendingRequests.
// TODO Create action class that has function RemovePendingTargetRequest as default implementation.
// TODO reverting confirm or reject does not result in same action, if the action would be modified, e.g.
// only part of value is accepted as gift the original action would not be restorable. In case of adding
// new option that makes partial accept, confirm for that action should be separate and it should extend parameter
// count with the original and changed value.
// TODO update modifiers to use predicates to minimize code
// TODO consider that action like responses or deal creation could be using sequence of actions e.g. composite
// this would allow them to do only one thing, while allowing additional effects to take place via consequent actions.
// TODO reevaluate UniqueId and SourceUniqueId based on composite actions and revert. Currently UniqueId is never used as
// it's available always the same way SourceUniqueId is and SourceUniqueId is universal identifier for response actions.
// This makes SDID more suitable in current code base as main Id during creating entities, while UniqueId has no role
// other than synchronization of Client and Server in batch.


bool IUDActionInterface::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default Interface call is checking only ActionId and ParameterCount.
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Verifying Execution Action[Id(%d)==(%d), ValueCount(%d)==(%d)]."),
		targetWorldState->PerspectivePlayerId, actionData.ActionTypeId, GetActionTypeId(),
		actionData.ValueParameters.Num(), GetRequiredParametersCount());
	
	return actionData.ActionTypeId == GetActionTypeId() &&
		actionData.ValueParameters.Num() == GetRequiredParametersCount();
}	 

void IUDActionInterface::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default Interface call is empty with a log.
	UE_LOG(
		LogTemp, 
		Log, 
		TEXT("INSTANCE(%d): Execuing Action[Id(%d), Invoker(%d), UID(%d), Source(%d), ValueCount(%d), TextLength(%d)]."),
		targetWorldState->PerspectivePlayerId, 
		actionData.ActionTypeId, 
		actionData.InvokerPlayerId,
		actionData.UniqueId,
		actionData.SourceUniqueId,
		actionData.ValueParameters.Num(),
		actionData.TextParameter.Len()
	);
}

void IUDActionInterface::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default Interface call is empty with a log.
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Execuing Action[Id(%d), Invoker(%d), UID(%d), Source(%d), ValueCount(%d), TextLength(%d)]."),
		targetWorldState->PerspectivePlayerId,
		actionData.ActionTypeId,
		actionData.InvokerPlayerId,
		actionData.UniqueId,
		actionData.SourceUniqueId,
		actionData.ValueParameters.Num(),
		actionData.TextParameter.Len()
	);
}

TArray<FUDActionData> IUDActionInterface::GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState)
{
	TArray<FUDActionData> emptyArray;
	emptyArray.Empty(0);
	return emptyArray;
}

bool IUDActionInterface::RequiresBackup()
{
	// Default interface call always returns false as it does no backup.
	return false;
}

void IUDActionInterface::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Default interface call is empty and should never be invoked by correctly defined action.
}

/**
 * Removes pending request associated with action and specified target.
 */
void RemovePendingTargetRequest(FUDActionData actionData, int32 targetId, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Deleting request(%d)"),
		targetWorldState->PerspectivePlayerId, 
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num(),
		actionData.SourceUniqueId
	);
	
	// Item is removed based on the key.
	targetWorldState->Players[targetId]->PendingRequests.Remove(actionData.SourceUniqueId);

	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		targetWorldState->PerspectivePlayerId,
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num()
	);
}

/**
 * Adds pending request associated with action and specified target.
 */
void AddPendingTargetRequest(FUDActionData actionData, int32 targetId, TObjectPtr<UUDWorldState> targetWorldState)
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Adding request(%d)"),
		targetWorldState->PerspectivePlayerId,
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num(),
		actionData.SourceUniqueId
	);

	// Item is added as key value pair.
	targetWorldState->Players[targetId]->PendingRequests.Add(actionData.SourceUniqueId, actionData);

	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		targetWorldState->PerspectivePlayerId,
		targetId,
		targetWorldState->Players[targetId]->PendingRequests.Num()
	);
}