// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldSimulation.h"

void AUDWorldSimulation::CreateState(int32 playerId, bool isPlayerOrAi)
{
	if (playerId == GaiaWorldStateId || States.Contains(playerId))
	{
		UE_LOG(LogTemp, Log, TEXT("Duplicate initialization of player state for player with id: %d"), playerId);
		return;
	}
	TObjectPtr<UUDWorldState> newState = UUDWorldState::CreateState(playerId, isPlayerOrAi);
	States.Add(playerId, newState);
}

void AUDWorldSimulation::InitializeGaiaWorldState()
{
	TObjectPtr<UUDWorldState> newState = UUDWorldState::CreateState(GaiaWorldStateId, false);
	States.Add(GaiaWorldStateId, newState);
	GaiaState = States[GaiaWorldStateId];
}

void AUDWorldSimulation::RegisterAction(TObjectPtr<IUDActionInterface> newAction)
{
	if (Actions.Contains(newAction->GetActionTypeId()))
	{
		UE_LOG(LogTemp, Log, TEXT("Duplicate registration of action with id: %d"), newAction->GetActionTypeId());
	}
	Actions.Add(newAction->GetActionTypeId(), newAction);
}