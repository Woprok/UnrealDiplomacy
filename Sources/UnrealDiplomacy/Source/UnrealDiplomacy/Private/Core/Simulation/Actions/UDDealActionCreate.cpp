// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionCreate.h"
#include "Core/Simulation/UDWorldState.h"
/**
 * Deal can be created only if all other deals are closed.
 * TODO bugfix issues with mutliple active deals owned by single player?
 * Currently it prevents only from having multiple deals in opened state...
 */
bool UUDDealActionCreate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool result = IUDActionInterface::CanExecute(action, world);
	if (result)
	{
		bool canCreate = true;
		for (auto deal : world->Deals)
		{
			if (deal.Value->DealSimulationResult == EUDDealSimulationResult::Opened &&
				deal.Value->OwnerUniqueId == action.InvokerPlayerId)
			{
				canCreate = false;
			}
		}
		result = result && canCreate;
	}
	return result;
}

void UUDDealActionCreate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Creates new deal state with Id same as this action SourceUniqueId.
	world->Deals.Add(action.SourceUniqueId,
		UUDDealState::CreateState(action.SourceUniqueId, action.InvokerPlayerId));
	world->Deals[action.SourceUniqueId]->Participants.Add(action.InvokerPlayerId);
}

void UUDDealActionCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Removes deal with key equal to SourceUniqueId of the original action.
	world->Deals[action.SourceUniqueId]->Participants.Remove(action.InvokerPlayerId);
	world->Deals.Remove(action.SourceUniqueId);
}