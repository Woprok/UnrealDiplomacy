// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldSimulation.h"

void AUDWorldSimulation::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Initializing."));
	WorldGenerator = NewObject<UUDWorldGenerator>(this);
	ModifierManager = NewObject<UUDModifierManager>(this);
	LoadCoreActions();
}

void AUDWorldSimulation::CreateState(int32 playerId, bool isPlayerOrAi)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Creating new State."));
	if (States.Contains(playerId))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Duplicate initialization of player state for player with id(%d)."), playerId);
		return;
	}

	if (!isPlayerOrAi && playerId == UUDWorldState::GaiaWorldStateId)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Gaia as Id(%d)."), playerId);
	}
	else if (isPlayerOrAi && playerId != UUDWorldState::GaiaWorldStateId)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Player or Ai as Id(%d)."), playerId);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Invalid combination of Id(%d) and isPlayerOrAi."), playerId);
		return;
	}

	TObjectPtr<UUDWorldState> newState = UUDWorldState::CreateState(playerId, isPlayerOrAi);
	States.Add(playerId, newState);
	SynchronizeNewPlayerState(newState);
}

void AUDWorldSimulation::RegisterAction(TObjectPtr<IUDActionInterface> newAction)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Action."));
	if (Actions.Contains(newAction->GetActionTypeId()))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Duplicate registration of action with id(%d)."), newAction->GetActionTypeId());
		return;
	}
	newAction->SetWorldGenerator(WorldGenerator);
	newAction->SetModifierManager(ModifierManager);
	Actions.Add(newAction->GetActionTypeId(), newAction.Get());
}

void AUDWorldSimulation::ExecuteAction(FUDActionData& newAction)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Executing Action."));
	if (!Actions.Contains(newAction.ActionTypeId))
	{
		// Blocked execution of non-existing action.
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action executor for action id(%d) is not defined."), newAction.ActionTypeId);
		return;
	}
	// Obtained executor for this action.
	auto& actionExecutor = Actions[newAction.ActionTypeId];
	
	if (!actionExecutor->CanExecute(newAction, GetSpecificState(UUDWorldState::GaiaWorldStateId)))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action executor was halted for action id(%d) due to executor id(%d)."), 
			newAction.ActionTypeId, actionExecutor->GetActionTypeId());
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
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Synchrinizing new Player."));
	// New player state must be synchronzied from old action list first.
	for (auto& actionData : ExecutionHistory)
	{
		Actions[actionData.ActionTypeId]->Execute(actionData, newState);		
	}
	// After that push new synchronize action to all, including new joined player.
	FUDActionData joinPlayer(UUDAddPlayerAction::ActionTypeId, newState->PerspectivePlayerId);
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Calling join action for player id(%d)."), newState->PerspectivePlayerId);
	ExecuteAction(joinPlayer);
}

void AUDWorldSimulation::RevertAction()
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Reverting Action."));
	if (ExecutionHistory.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action executor couldn't unload action due to empty history."));
		return;
	}
	FUDActionData oldAction = ExecutionHistory.Pop();

	// Revert all current states with this action.
	for (auto& pair : States)
	{
		Actions[oldAction.ActionTypeId]->Revert(oldAction, pair.Value);
	}
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action executor reverted action last successful action."));
	UndoHistory.Add(oldAction);
}

void AUDWorldSimulation::LoadCoreActions()
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Actions."));
	// Basics 0+
	//or this directly in the RegisterAction worked TObjectPtr<UUDLogAction> newAction2 = NewObject<UUDLogAction>(this);
	//TObjectPtr<UUDLogAction> log = NewObject<UUDLogAction>(this);
	//log->AddToRoot();
	//RegisterAction(log);
	//LogAction = NewObject<UUDLogAction>(this);
	//RegisterAction(LogAction);
	//RegisterAction(NewObject<UUDLogAction>(this));
	/*RegisterAction(NewObject<UUDAddPlayerAction>(this));
	RegisterAction(NewObject<UUDStartGameAction>(this));
	RegisterAction(NewObject<UUDEndTurnAction>(this));
	// Gaia 100+
	RegisterAction(NewObject<UUDGenerateIncomeAction>(this));
	// Player 1000+
	RegisterAction(NewObject<UUDUnconditionalGiftAction>(this));
	// Gift Action 1001-1003
	RegisterAction(NewObject<UUDGiftAction>(this));
	RegisterAction(NewObject<UUDConfirmGiftAction>(this));
	RegisterAction(NewObject<UUDRejectGiftAction>(this));
	// CreateWorldMap action, requires self initializing WorldGenerator
	RegisterAction(NewObject<UUDCreateWorldMapAction>(this));
	// Take Tile
	RegisterAction(NewObject<UUDTakeTileAction>(this));
	// Exploit Tile
	RegisterAction(NewObject<UUDExploitTileAction>(this));
	// Transfer Tile Action 1004-1006
	RegisterAction(NewObject<UUDTransferTileAction>(this));
	RegisterAction(NewObject<UUDConfirmTransferTileAction>(this));
	RegisterAction(NewObject<UUDRejectTransferTileAction>(this));
	// Grant exploit permission
	RegisterAction(NewObject<UUDGrantExploitTilePermissionAction>(this));*/
}