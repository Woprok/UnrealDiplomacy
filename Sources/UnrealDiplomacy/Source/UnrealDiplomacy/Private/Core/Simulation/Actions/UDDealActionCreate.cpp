// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionCreate.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionCreate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isPlayer = action.InvokerFactionId != UUDGlobalData::GaiaFactionId;
	bool isNotModeratingDeal = !IsDealModerator(world, action.InvokerFactionId);
	bool isRegent = world->TurnData.RegentFaction == action.InvokerFactionId;
	return IUDActionInterface::CanExecute(action, world) && isPlayer && isNotModeratingDeal && isRegent;
}

void UUDDealActionCreate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Creates new deal state with Id same as this action SourceUniqueId.
	world->Deals.Add(action.SourceUniqueId, UUDDealState::CreateState(action.SourceUniqueId, action.InvokerFactionId));
	world->Deals[action.SourceUniqueId]->Participants.Add(action.InvokerFactionId);
	FString factionName = world->Factions[action.InvokerFactionId]->Name;
	world->Deals[action.SourceUniqueId]->Name = FString::Format(TEXT("{0} Deal {1}"), { factionName, action.SourceUniqueId });
}

void UUDDealActionCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Removes deal with key equal to SourceUniqueId of the original action.
	world->Deals[action.SourceUniqueId]->Participants.Remove(action.InvokerFactionId);
	world->Deals.Remove(action.SourceUniqueId);
}

bool UUDDealActionCreate::IsDealModerator(TObjectPtr<UUDWorldState> world, int32 playerId) const
{
	for (auto deal : world->Deals)
	{
		if (deal.Value->DealSimulationResult == EUDDealSimulationResult::Opened &&
			deal.Value->OwnerUniqueId == playerId)
		{
			return true;
		}
	}
	return false;
}