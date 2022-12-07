// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldSimulation.h"

void AUDWorldSimulation::Initialize()
{
	WorldGenerator = NewObject<UUDWorldGenerator>();
	ModifierManager = NewObject<UUDModifierManager>();
	LoadCoreActions();
}

void AUDWorldSimulation::CreateState(int32 playerId, bool isPlayerOrAi)
{
	if (States.Contains(playerId))
	{
		UE_LOG(LogTemp, Log, TEXT("Duplicate initialization of player state for player with id(%d)."), playerId);
		return;
	}

	if (isPlayerOrAi && playerId != UUDWorldState::GaiaWorldStateId)
	{
		UE_LOG(LogTemp, Log, TEXT("Registering Player or Ai as Id(%d)."), playerId);
	}
	else if (!isPlayerOrAi && playerId == UUDWorldState::GaiaWorldStateId)
	{
		UE_LOG(LogTemp, Log, TEXT("Registering Gaia as Id(%d)."), playerId);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Invalid combination of Id(%d) and isPlayerOrAi."), playerId);
		return;
	}

	TObjectPtr<UUDWorldState> newState = UUDWorldState::CreateState(playerId, isPlayerOrAi);
	States.Add(playerId, newState);
	SynchronizeNewPlayerState(newState);
}

void AUDWorldSimulation::RegisterAction(TObjectPtr<IUDActionInterface> newAction)
{
	if (Actions.Contains(newAction->GetActionTypeId()))
	{
		UE_LOG(LogTemp, Log, TEXT("Duplicate registration of action with id(%d)."), newAction->GetActionTypeId());
	}
	newAction->SetWorldGenerator(WorldGenerator);
	newAction->SetModifierManager(ModifierManager);
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
	if (!actionExecutor->CanExecute(newAction, GetSpecificState(UUDWorldState::GaiaWorldStateId)))
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
	FUDActionData joinPlayer(UUDAddPlayerAction::ActionTypeId, newState->PerspectivePlayerId);
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

void AUDWorldSimulation::LoadCoreActions()
{
	// Basics 0+
	RegisterAction(NewObject<UUDLogAction>());
	RegisterAction(NewObject<UUDAddPlayerAction>());
	RegisterAction(NewObject<UUDStartGameAction>());
	RegisterAction(NewObject<UUDEndTurnAction>());
	// Gaia 100+
	RegisterAction(NewObject<UUDGenerateIncomeAction>());
	// Player 1000+
	RegisterAction(NewObject<UUDUnconditionalGiftAction>());
	// Gift Action 1001-1003
	RegisterAction(NewObject<UUDGiftAction>());
	RegisterAction(NewObject<UUDConfirmGiftAction>());
	RegisterAction(NewObject<UUDRejectGiftAction>());
	// CreateWorldMap action, requires self initializing WorldGenerator
	RegisterAction(NewObject<UUDCreateWorldMapAction>());
	// Take Tile
	RegisterAction(NewObject<UUDTakeTileAction>());
	// Exploit Tile
	RegisterAction(NewObject<UUDExploitTileAction>());
	// Transfer Tile Action 1004-1006
	RegisterAction(NewObject<UUDTransferTileAction>());
	RegisterAction(NewObject<UUDConfirmTransferTileAction>());
	RegisterAction(NewObject<UUDRejectTransferTileAction>());
	// Grant exploit permission
	RegisterAction(NewObject<UUDGrantExploitTilePermissionAction>());
}