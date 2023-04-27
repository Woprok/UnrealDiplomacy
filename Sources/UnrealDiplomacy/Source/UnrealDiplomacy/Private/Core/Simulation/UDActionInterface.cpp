// Copyright Miroslav Valach

// TODO LIST
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

#include "Core/Simulation/UDActionInterface.h"

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

#pragma region State and Result of Deal Action

bool UUDAdvanceStateAssemblingDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateAssemblingDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::CreatingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateAssemblingDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateAssemblingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}
void UUDAdvanceStateAssemblingDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateAssemblingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::CreatingDraft;
}

bool UUDAdvanceStateExtendingDraftDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateExtendingDraftDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Assembling;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateExtendingDraftDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateExtendingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ExtendingDraft;
}
void UUDAdvanceStateExtendingDraftDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateExtendingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Assembling;
}

bool UUDAdvanceStateDemandsAndRequestsDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateDemandsAndRequestsDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::ExtendingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateDemandsAndRequestsDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateDemandsAndRequestsDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::DemandsAndRequests;
}
void UUDAdvanceStateDemandsAndRequestsDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateDemandsAndRequestsDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::ExtendingDraft;
}

bool UUDAdvanceStateBiddingDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateBiddingDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::DemandsAndRequests;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateBiddingDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateBiddingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Bidding;
}
void UUDAdvanceStateBiddingDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateBiddingDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::DemandsAndRequests;
}

bool UUDAdvanceStateFinalizingDraftDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateFinalizingDraftDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Bidding;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateFinalizingDraftDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateFinalizingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::FinalizingDraft;
}
void UUDAdvanceStateFinalizingDraftDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateFinalizingDraftDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Bidding;
}

bool UUDAdvanceStateVoteDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateVoteDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::FinalizingDraft;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateVoteDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Vote;
}
void UUDAdvanceStateVoteDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::FinalizingDraft;
}

bool UUDAdvanceStateResolutionDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateResolutionDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Vote;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateResolutionDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResolutionDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Resolution;
}
void UUDAdvanceStateResolutionDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResolutionDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Vote;
}

bool UUDAdvanceStateResultDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceStateResultDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::Resolution;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceStateResultDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResultDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Result;
}
void UUDAdvanceStateResultDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceStateResultDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationState = EUDDealSimulationState::Resolution;
}

bool UUDAdvanceResultPassedDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultPassedDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultPassedDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultPassedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Passed;
}
void UUDAdvanceResultPassedDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultPassedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

bool UUDAdvanceResultVetoedDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultVetoedDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultVetoedDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultVetoedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Vetoed;
}
void UUDAdvanceResultVetoedDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDAdvanceResultVetoedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

bool UUDAdvanceResultDisassembledDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultDisassembledDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultDisassembledDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultDisassembledDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Disassembled;
}
void UUDAdvanceResultDisassembledDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultDisassembledDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

bool UUDAdvanceResultClosedDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAdvanceResultClosedDealAction::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDAdvanceResultClosedDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultClosedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Closed;
}
void UUDAdvanceResultClosedDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDAdvanceResultClosedDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}

#pragma endregion

#pragma region Discussion Point

bool UUDAddDiscussionItemDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDAddDiscussionItemDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddDiscussionItemDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new point.
	FUDDealData data = UUDAddDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Add(actionData.SourceUniqueId, UUDDiscussionItem::CreateState(actionData.InvokerPlayerId));
	targetWorldState->Deals[data.DealId]->PrimaryPoints.Add(actionData.SourceUniqueId);
}
void UUDAddDiscussionItemDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Deletes created point that is currently located at the end.
	FUDDealData data = UUDAddDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Remove(actionData.SourceUniqueId);
	targetWorldState->Deals[data.DealId]->PrimaryPoints.Remove(actionData.SourceUniqueId);
}

bool UUDIgnoreDiscussionItemDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDIgnoreDiscussionItemDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		bool isNotIgnored = targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored == false;
		result = result && isStateOpen && isResultOpen && isNotIgnored;
	}
	return result;
}
void UUDIgnoreDiscussionItemDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Mark point as ignored. Removing is not necessary.
	// TODO consider Id based indexing for Points...
	FUDDealPointData data = UUDIgnoreDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored = true;
}
void UUDIgnoreDiscussionItemDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Mark point as available for editing and use.
	FUDDealPointData data = UUDIgnoreDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->IsIgnored = false;
}

bool UUDSendMessageDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDSendMessageDealAction::ConvertData(actionData);
		bool isNotEmpty = actionData.TextParameter.Len() > 0;
		result = result && isNotEmpty;
	}
	return result;
}
void UUDSendMessageDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Add to history.
	FUDDealData data = UUDSendMessageDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->ChatHistory.Add(actionData.TextParameter);
}
void UUDSendMessageDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Remove last add message from the history.
	FUDDealData data = UUDSendMessageDealAction::ConvertData(actionData);
	int32 lastItem = targetWorldState->Deals[data.DealId]->ChatHistory.Num() - 1;
	targetWorldState->Deals[data.DealId]->ChatHistory.RemoveAt(lastItem);
}

bool UUDAddChildDiscussionItemDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDAddChildDiscussionItemDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddChildDiscussionItemDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new sub-point for a specified Point with current SourceUniqueId.
	FUDDealPointData data = UUDAddChildDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Add(actionData.SourceUniqueId, UUDDiscussionItem::CreateState(actionData.InvokerPlayerId));
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Consequencies.Add(actionData.SourceUniqueId);
}
void UUDAddChildDiscussionItemDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Deletes created sub-point that is saved with this action SourceUniqueId.
	FUDDealPointData data = UUDAddChildDiscussionItemDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points.Remove(actionData.SourceUniqueId);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Consequencies.Remove(actionData.SourceUniqueId);
}

#pragma endregion

#pragma region Deal Item Updates

bool UUDDefineActionDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDefineActionDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDefineActionDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointValueData newData = UUDDefineActionDealAction::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ActionId = newData.Value;
}
void UUDDefineActionDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old value.
	FUDDealPointValueData newData = UUDDefineActionDealAction::ConvertData(actionData);
	FUDValueData oldData = UUDDefineActionDealAction::ConvertBackupData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ActionId = oldData.Value;
}
bool UUDDefineActionDealAction::RequiresBackup()
{
	return true;
}
void UUDDefineActionDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDefineActionDealAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	actionData.BackupValueParameters.Add(targetWorldState->Deals[data.DealId]->Points[data.Point]->ActionId);
}

TArray<FUDActionData> UUDDefineActionDealAction::GetSubactions(FUDActionData& parentAction, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealPointValueData data = UUDDefineActionDealAction::ConvertData(parentAction);
	return {
		FUDActionData(
			UUDCleanParametersPointDealAction::ActionTypeId,
			parentAction.InvokerPlayerId,
			{ data.DealId, data.Point }
		)
	};
}

bool UUDCleanParametersPointDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointData data = UUDCleanParametersPointDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDCleanParametersPointDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointData newData = UUDCleanParametersPointDealAction::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->TextParameter.Empty(0);
}
void UUDCleanParametersPointDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointData newData = UUDCleanParametersPointDealAction::ConvertData(actionData);
	// Old data are just pasted as only original action would be able to determine what they were.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Append(actionData.BackupValueParameters);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->TextParameter.Append(actionData.TextParameter);
}
bool UUDCleanParametersPointDealAction::RequiresBackup()
{
	return true;
}
void UUDCleanParametersPointDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointData data = UUDCleanParametersPointDealAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	actionData.TextParameter.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->TextParameter);
}


bool UUDChangeValueParameterPointDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDChangeValueParameterPointDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDChangeValueParameterPointDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value, we expect it to be empty due to cleanup sequence.
	FUDDealPointValueData newData = UUDChangeValueParameterPointDealAction::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Value);
}
void UUDChangeValueParameterPointDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointValueData newData = UUDChangeValueParameterPointDealAction::ConvertData(actionData);
	FUDValueData oldData = UUDChangeValueParameterPointDealAction::ConvertBackupData(actionData);
	// Old data are replacing the new.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Value);
}
bool UUDChangeValueParameterPointDealAction::RequiresBackup()
{
	return true;
}
void UUDChangeValueParameterPointDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old param is backuped for future revert use.
	FUDDealPointValueData data = UUDChangeValueParameterPointDealAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	if (targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters.Num() == 0)
	{
		actionData.BackupValueParameters.Append( { 0 } );
	}
	else
	{
		actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	}
}

bool UUDChangeTileParameterPointDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointTileData data = UUDChangeTileParameterPointDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDChangeTileParameterPointDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointTileData newData = UUDChangeTileParameterPointDealAction::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Y);
}
void UUDChangeTileParameterPointDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointTileData newData = UUDChangeTileParameterPointDealAction::ConvertData(actionData);
	FUDTileData oldData = UUDChangeTileParameterPointDealAction::ConvertBackupData(actionData);
	// Old data are replacing the new.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Y);
}
bool UUDChangeTileParameterPointDealAction::RequiresBackup()
{
	return true;
}
void UUDChangeTileParameterPointDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old param is backuped for future revert use.
	FUDDealPointTileData data = UUDChangeTileParameterPointDealAction::ConvertData(actionData);	
	actionData.BackupValueParameters.Empty(0);
	if (targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters.Num() == 0)
	{
		actionData.BackupValueParameters.Append({ 0, 0 });
	}
	else
	{
		actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	}
}

bool UUDChangeTileValueParameterPointDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointTileValueData data = UUDChangeTileValueParameterPointDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDChangeTileValueParameterPointDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointTileValueData newData = UUDChangeTileValueParameterPointDealAction::ConvertData(actionData);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Y);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(newData.Value);
}
void UUDChangeTileValueParameterPointDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Change to old values.
	FUDDealPointTileValueData newData = UUDChangeTileValueParameterPointDealAction::ConvertData(actionData);
	FUDTileValueData oldData = UUDChangeTileValueParameterPointDealAction::ConvertBackupData(actionData);
	// Old data are replacing the new.
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Empty(0);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.X);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Y);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->ValueParameters.Add(oldData.Value);
}
bool UUDChangeTileValueParameterPointDealAction::RequiresBackup()
{
	return true;
}
void UUDChangeTileValueParameterPointDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old param is backuped for future revert use.
	FUDDealPointTileValueData data = UUDChangeTileValueParameterPointDealAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	if (targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters.Num() == 0)
	{
		actionData.BackupValueParameters.Append({ 0, 0, 0 });
	}
	else
	{
		actionData.BackupValueParameters.Append(targetWorldState->Deals[data.DealId]->Points[data.Point]->ValueParameters);
	}
}









bool UUDDefinePointTypeDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDDefinePointTypeDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDDefinePointTypeDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Change to new value.
	FUDDealPointValueData newData = UUDDefinePointTypeDealAction::ConvertData(actionData);
	EUDPointType pointType = UUDDefinePointTypeDealAction::IntegerToPointType(newData.Value);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->Type = pointType;
}
void UUDDefinePointTypeDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);	
	// Change to old value.
	FUDDealPointValueData newData = UUDDefinePointTypeDealAction::ConvertData(actionData);
	FUDValueData oldData = UUDDefinePointTypeDealAction::ConvertBackupData(actionData);
	EUDPointType pointType = UUDDefinePointTypeDealAction::IntegerToPointType(oldData.Value);
	targetWorldState->Deals[newData.DealId]->Points[newData.Point]->Type = pointType;
}
bool UUDDefinePointTypeDealAction::RequiresBackup()
{
	return true;
}
void UUDDefinePointTypeDealAction::Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	// Old action is backuped for future revert use.
	FUDDealPointValueData data = UUDDefinePointTypeDealAction::ConvertData(actionData);
	actionData.BackupValueParameters.Empty(0);
	int32 pointType = UUDDefinePointTypeDealAction::PointTypeToInteger(targetWorldState->Deals[data.DealId]->Points[data.Point]->Type);
	actionData.BackupValueParameters.Add(pointType);
}

bool UUDAddInvokerDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDAddInvokerDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddInvokerDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Add(data.Value);
}
void UUDAddInvokerDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Remove(data.Value);
}

bool UUDRemoveInvokerDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDRemoveInvokerDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDRemoveInvokerDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Remove(data.Value);
}
void UUDRemoveInvokerDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveInvokerDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Invokers.Add(data.Value);
}

bool UUDAddTargetDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDAddTargetDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDAddTargetDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Add(data.Value);
}
void UUDAddTargetDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDAddTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Remove(data.Value);
}

bool UUDRemoveTargetDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealPointValueData data = UUDRemoveTargetDealAction::ConvertData(actionData);
		bool isStateOpen = targetWorldState->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::FinalizingDraft;
		bool isResultOpen = targetWorldState->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
		result = result && isStateOpen && isResultOpen;
	}
	return result;
}
void UUDRemoveTargetDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Remove(data.Value);
}
void UUDRemoveTargetDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealPointValueData data = UUDRemoveTargetDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->Points[data.Point]->Targets.Add(data.Value);
}


bool UUDReadyDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDReadyDealAction::ConvertData(actionData);
		bool isNotReady = !targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isNotReady;
	}
	return result;
}
void UUDReadyDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Add(actionData.InvokerPlayerId);
}
void UUDReadyDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Remove(actionData.InvokerPlayerId);
}

bool UUDNotReadyDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDNotReadyDealAction::ConvertData(actionData);
		bool isReady = targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isReady;
	}
	return result;
}
void UUDNotReadyDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDNotReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Remove(actionData.InvokerPlayerId);
}
void UUDNotReadyDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDNotReadyDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->IsReadyPlayerList.Add(actionData.InvokerPlayerId);
}


bool UUDPositiveVoteDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDPositiveVoteDealAction::ConvertData(actionData);
		bool isNotReady = !targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isNotReady;
	}
	return result;
}
void UUDPositiveVoteDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDPositiveVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Add(actionData.InvokerPlayerId);
}
void UUDPositiveVoteDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDPositiveVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Remove(actionData.InvokerPlayerId);
}

bool UUDNegativeVoteDealAction::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDNegativeVoteDealAction::ConvertData(actionData);
		bool isReady = targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Contains(actionData.InvokerPlayerId);
		result = result && isReady;
	}
	return result;
}
void UUDNegativeVoteDealAction::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	FUDDealData data = UUDNegativeVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Remove(actionData.InvokerPlayerId);
}
void UUDNegativeVoteDealAction::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	FUDDealData data = UUDNegativeVoteDealAction::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->PositiveVotePlayerList.Add(actionData.InvokerPlayerId);
}
#pragma endregion
