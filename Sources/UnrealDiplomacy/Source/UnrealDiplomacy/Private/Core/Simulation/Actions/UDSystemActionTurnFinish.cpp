// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionTurnFinish.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDSystemActionIntermezzoStart.h"

bool UUDSystemActionTurnFinish::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isPlaying = !world->TurnData.TurnFinishedFactions.Contains(action.InvokerFactionId);
	return IUDActionInterface::CanExecute(action, world) && isPlaying;
}

void UUDSystemActionTurnFinish::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Finish turn by adding to set
	world->TurnData.TurnFinishedFactions.Add(action.InvokerFactionId);
}

void UUDSystemActionTurnFinish::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert finish by removing from set.
	world->TurnData.TurnFinishedFactions.Remove(action.InvokerFactionId);
}

TArray<FUDActionData> UUDSystemActionTurnFinish::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	// Create and return single intermezzo action in name of gaia faction.
	TArray<FUDActionData> intermezzo = { };
	if (world->TurnData.FactionTurnOrder.Num() == world->TurnData.TurnFinishedFactions.Num()) 
	{
		intermezzo.Add(FUDActionData(UUDSystemActionIntermezzoStart::ActionTypeId, UUDGlobalData::GaiaFactionId));
	}
	return intermezzo;
}