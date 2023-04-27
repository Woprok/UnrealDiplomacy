// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionCreate.h"

/**
 * Deal can be created only if all other deals are closed.
 * TODO bugfix issues with mutliple active deals owned by single player?
 * Currently it prevents only from having multiple deals in opened state...
 */
bool UUDDealActionCreate::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		bool canCreate = true;
		for (auto deal : targetWorldState->Deals)
		{
			if (deal.Value->DealSimulationResult == EUDDealSimulationResult::Opened &&
				deal.Value->OwnerUniqueId == actionData.InvokerPlayerId)
			{
				canCreate = false;
			}
		}
		result = result && canCreate;
	}
	return result;
}

void UUDDealActionCreate::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Execute(actionData, targetWorldState);
	// Creates new deal state with Id same as this action SourceUniqueId.
	targetWorldState->Deals.Add(actionData.SourceUniqueId,
		UUDDealState::CreateState(actionData.SourceUniqueId, actionData.InvokerPlayerId));
	targetWorldState->Deals[actionData.SourceUniqueId]->Participants.Add(actionData.InvokerPlayerId);
}

void UUDDealActionCreate::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	IUDActionInterface::Revert(actionData, targetWorldState);
	// Removes deal with key equal to SourceUniqueId of the original action.
	targetWorldState->Deals[actionData.SourceUniqueId]->Participants.Remove(actionData.InvokerPlayerId);
	targetWorldState->Deals.Remove(actionData.SourceUniqueId);
}