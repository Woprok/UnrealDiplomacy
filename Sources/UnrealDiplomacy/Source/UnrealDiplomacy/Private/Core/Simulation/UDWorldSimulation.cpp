// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldSimulation.h"

void AUDWorldSimulation::Initialize()
{
	WorldGenerator = NewObject<UUDWorldGenerator>();
}

void AUDWorldSimulation::CreateState(int32 playerId, bool isPlayerOrAi)
{
	UE_LOG(LogTemp, Log, TEXT("Id(%d)."), playerId);
	if (playerId == UUDWorldState::GaiaWorldStateId || States.Contains(playerId))
	{
		UE_LOG(LogTemp, Log, TEXT("Duplicate initialization of player state for player with id(%d)."), playerId);
		return;
	}
	TObjectPtr<UUDWorldState> newState = UUDWorldState::CreateState(playerId, isPlayerOrAi);
	States.Add(playerId, newState);
	SynchronizeNewPlayerState(newState);
}

void AUDWorldSimulation::InitializeGaiaWorldState(int32 gaiaId)
{
	// Let's leave this here, so we don't break it and if we do, then let's pray we find it :)
	check(gaiaId == UUDWorldState::GaiaWorldStateId);

	TObjectPtr<UUDWorldState> newState = UUDWorldState::CreateState(gaiaId, false);
	States.Add(gaiaId, newState);
	GaiaState = States[gaiaId];
	SynchronizeNewPlayerState(newState);
}

void AUDWorldSimulation::RegisterActionMaker(TObjectPtr<IUDActionHandlingInterface> newListener)
{
	newListener->OnActionDecidedDelegate.BindUObject(this, &AUDWorldSimulation::ExecuteAction);
}

void AUDWorldSimulation::RegisterAction(TObjectPtr<IUDActionInterface> newAction)
{
	if (Actions.Contains(newAction->GetActionTypeId()))
	{
		UE_LOG(LogTemp, Log, TEXT("Duplicate registration of action with id(%d)."), newAction->GetActionTypeId());
	}
	newAction->SetWorldGenerator(WorldGenerator);
	Actions.Add(newAction->GetActionTypeId(), newAction);
}

void AUDWorldSimulation::ExecuteAction(FUDActionData& newAction)
{
	if (!Actions.Contains(newAction.ActionTypeId))
	{
		// Blocked execution of non-existing action.
		UE_LOG(LogTemp, Log, TEXT("Action executor for action id(%d) is not defined."), newAction.ActionTypeId);
		return;
	}
	// Obtained executor for this action.
	auto& actionExecutor = Actions[newAction.ActionTypeId];
	if (!actionExecutor->CanExecute(newAction, GaiaState))
	{
		UE_LOG(LogTemp, Log, TEXT("Action executor was halted for action id(%d)."), newAction.ActionTypeId);
		return;
	}

	if (!IsValidAssignableActionId(newAction.UniqueId))
	{
		newAction.UniqueId = GetAssignableActionId();
	}

	// Saved for future reference
	ExecutionHistory.Add(newAction);
	// Updated all current states with this action.
	for (auto& pair : States)
	{
		Actions[newAction.ActionTypeId]->Execute(newAction, pair.Value);
	}
	// Notifies everyone about the action.
	OnBroadcastActionExecutedDelegate.Broadcast(newAction);
}

void AUDWorldSimulation::SynchronizeNewPlayerState(TObjectPtr<UUDWorldState> newState)
{
	// New player state must be synchronzied from old action list first.
	for (auto& actionData : ExecutionHistory)
	{
		auto& actionExecutor = Actions[actionData.ActionTypeId];
		Actions[actionData.ActionTypeId]->Execute(actionData, newState);		
	}
	// After that push new synchronize action to all, including new joined player.
	FUDActionData joinPlayer(1, newState->PerspectivePlayerId);
	ExecuteAction(joinPlayer);
}

void AUDWorldSimulation::RevertAction()
{
	if (ExecutionHistory.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Action executor couldn't unload action due to empty history."));
		return;
	}
	FUDActionData oldAction = ExecutionHistory.Pop();

	// Obtained executor for this action.
	auto& actionExecutor = Actions[oldAction.ActionTypeId];
	// Revert all current states with this action.
	for (auto& pair : States)
	{
		Actions[oldAction.ActionTypeId]->Revert(oldAction, pair.Value);
	}
	UE_LOG(LogTemp, Log, TEXT("Action executor reverted action last successful action."));
	UndoHistory.Add(oldAction);
}