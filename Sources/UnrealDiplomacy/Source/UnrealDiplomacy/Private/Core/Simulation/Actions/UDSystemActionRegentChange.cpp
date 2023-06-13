// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionRegentChange.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDSystemActionIntermezzoEnd.h"

bool UUDSystemActionRegentChange::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isGaia = action.InvokerFactionId == UUDGlobalData::GaiaFactionId;
	return IUDActionInterface::CanExecute(action, world) && isGaia;
}

void UUDSystemActionRegentChange::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Pass regency to next player.
	NextRegent(world);
}

void UUDSystemActionRegentChange::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Return regency to last player.
	PreviousRegent(world);
}

void UUDSystemActionRegentChange::NextRegent(TObjectPtr<UUDWorldState> world)
{
	// Find regent player successor.
	int32 nextPlayerId = 0;
	for (auto& playerUniqueId : world->TurnData.FactionTurnOrder)
	{
		if (playerUniqueId > world->TurnData.RegentFaction)
		{
			nextPlayerId = playerUniqueId;
			break;
		}
	}
	// Update regent player and counter.
	world->TurnData.RegentFaction = nextPlayerId;
	world->TurnData.Turn += 1;
}

void UUDSystemActionRegentChange::PreviousRegent(TObjectPtr<UUDWorldState> world)
{
	// Get last player for the case of previous player being at the end of chain.
	int32 lastPlayerId = world->TurnData.FactionTurnOrder[world->TurnData.FactionTurnOrder.Num() - 1];
	for (auto& playerUniqueId : world->TurnData.FactionTurnOrder)
	{
		// Once we find current regent we end the search and return previous element.
		if (playerUniqueId == world->TurnData.RegentFaction)
		{
			break;
		}
		lastPlayerId = playerUniqueId;
	}
	// Revert regent player and counter.
	world->TurnData.RegentFaction = lastPlayerId;
	world->TurnData.Turn -= 1;
}

TArray<FUDActionData> UUDSystemActionRegentChange::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	// Create and return single intermezzo action in name of gaia faction.
	TArray<FUDActionData> intermezzo = {
		FUDActionData(UUDSystemActionIntermezzoEnd::ActionTypeId, UUDGlobalData::GaiaFactionId)
	};
	return intermezzo;
}