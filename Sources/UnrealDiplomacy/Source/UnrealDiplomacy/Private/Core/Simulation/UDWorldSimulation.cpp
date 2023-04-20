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
}

void AUDWorldSimulation::CreateStateAndSynchronize(int32 playerId, bool isPlayerOrAi)
{
	CreateState(playerId, isPlayerOrAi);
	SynchronizeNewPlayerState(States[playerId]);
	// After that push new synchronize action to all, including new joined player.
	FUDActionData joinPlayer(UUDAddPlayerAction::ActionTypeId, States[playerId]->PerspectivePlayerId);
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Calling join action for player id(%d)."), States[playerId]->PerspectivePlayerId);
	ExecuteAction(joinPlayer);
}

void AUDWorldSimulation::NaiveExecuteAction(FUDActionData& trustworthyAction)
{
	// This expects action data that passed through ExecuteAction and thus are valid and complete.
	// This avoids checks over full state, thus allowing execution even if full state is not present.
	// This also ignores simulation call for broadcast. Maybe this is actually useful ?
	// TODO move these comments and verify their trustworthyness.
	for (auto& pair : States)
	{
		Actions[trustworthyAction.ActionTypeId]->Execute(trustworthyAction, pair.Value);
	}
	OnBroadcastActionAppliedDelegate.Broadcast(trustworthyAction);
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
	// If action has non valid parent Id, then it's parent to self.
	// Inherited action already have parent from their constructor.
	// Thus only actions that are new/root do not have parent. And thus their parent is them.
	if (newAction.SourceUniqueId == 0)
	{
		newAction.SourceUniqueId = newAction.UniqueId;
	}

	if (actionExecutor->RequiresBackup())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Backup by UID(%d)."), newAction.UniqueId);
		actionExecutor->Backup(newAction, GetSpecificState(UUDWorldState::GaiaWorldStateId));
	}

	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action UID(%d)."), newAction.UniqueId);

	// Saved for future reference
	ExecutionHistory.Add(newAction);
	// Updated all current states with this action.
	NaiveExecuteAction(newAction);
	// Notifies everyone about the action.
	OnBroadcastVerifiedActionExecutedDelegate.Broadcast(newAction);
	// Continue notifying about the subsequent actions...
	if (actionExecutor->IsComposite())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Composite Action UID(%d) Started."), newAction.UniqueId);
		TArray<FUDActionData> subactions = actionExecutor->GetSubactions(newAction, GetSpecificState(UUDWorldState::GaiaWorldStateId));
		for (auto& action : subactions)
		{
			ExecuteAction(action);
		}
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Composite Action UID(%d) Finished."), newAction.UniqueId);
	}
}

void AUDWorldSimulation::SynchronizeNewPlayerState(TObjectPtr<UUDWorldState> newState)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Synchrinizing new Player."));
	// New player state must be synchronzied from old action list first.
	// This directly executes action over the supplied state.
	for (auto& actionData : ExecutionHistory)
	{
		Actions[actionData.ActionTypeId]->Execute(actionData, newState);
	}
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

FUDActionArray AUDWorldSimulation::GetHistoryUntil(int32 historyPoint)
{
	FUDActionArray newHistory;
	newHistory.Actions.SetNumZeroed(0);
	if (historyPoint == 0)
	{
		for (auto& historic : ExecutionHistory)
		{
			newHistory.Actions.Add(historic);
		}
	}
	else
	{
		for (auto& historic : ExecutionHistory)
		{
			if (historic.UniqueId < historyPoint)
			{
				newHistory.Actions.Add(historic);
			}
			else
			{
				break;
			}
		}
	}

	int32 first = 0;
	int32 last = 0;
	if (newHistory.Actions.Num() > 0)
	{
		first = newHistory.Actions[0].UniqueId;
		last = newHistory.Actions.Last().UniqueId;
	}

	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Returning %d historic actions from %d to %d."), newHistory.Actions.Num(), first, last);
	return newHistory;
}

void AUDWorldSimulation::RegisterAction(TScriptInterface<IUDActionInterface> newAction)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Action(%d)."), newAction->GetActionTypeId());
	if (Actions.Contains(newAction->GetActionTypeId()))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Duplicate registration of action with id(%d)."), newAction->GetActionTypeId());
		return;
	}
	newAction->SetWorldGenerator(WorldGenerator);
	newAction->SetModifierManager(ModifierManager);
	Actions.Add(newAction->GetActionTypeId(), newAction);
}

void AUDWorldSimulation::LoadCoreActions()
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Actions."));
	RegisterAction(NewObject<UUDLogAction>(this));
	RegisterAction(NewObject<UUDAddPlayerAction>(this));
	RegisterAction(NewObject<UUDStartGameAction>(this));
	RegisterAction(NewObject<UUDEndGameAction>(this));
	RegisterAction(NewObject<UUDEndTurnAction>(this));
	RegisterAction(NewObject<UUDForceEndTurnAction>(this));
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
	RegisterAction(NewObject<UUDGrantExploitTilePermissionAction>(this));
	// Deal actions 10000+
	RegisterAction(NewObject<UUDCreateDealAction>(this));
	RegisterAction(NewObject<UUDInviteParticipantDealAction>(this));
	RegisterAction(NewObject<UUDAcceptParticipationDealAction>(this));
	RegisterAction(NewObject<UUDRejectParticipationDealAction>(this));
	RegisterAction(NewObject<UUDKickParticipantDealAction>(this));
	RegisterAction(NewObject<UUDLeaveParticipationDealAction>(this));

	RegisterAction(NewObject<UUDAdvanceStateAssemblingDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceStateExtendingDraftDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceStateDemandsAndRequestsDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceStateBiddingDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceStateFinalizingDraftDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceStateVoteDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceStateResolutionDealAction>(this));

	RegisterAction(NewObject<UUDAdvanceResultPassedDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceResultVetoedDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceResultDisassembledDealAction>(this));
	RegisterAction(NewObject<UUDAdvanceResultClosedDealAction>(this));
	// Deal action update
	RegisterAction(NewObject<UUDAddDiscussionItemDealAction>(this));
	RegisterAction(NewObject<UUDIgnoreDiscussionItemDealAction>(this));
	RegisterAction(NewObject<UUDSendMessageDealAction>(this));
	RegisterAction(NewObject<UUDAddChildDiscussionItemDealAction>(this));
	RegisterAction(NewObject<UUDDefineActionDealAction>(this));
	RegisterAction(NewObject<UUDCleanParametersPointDealAction>(this));
	RegisterAction(NewObject<UUDChangeValueParameterPointDealAction>(this));
	RegisterAction(NewObject<UUDChangeTileParameterPointDealAction>(this));
	RegisterAction(NewObject<UUDChangeTileValueParameterPointDealAction>(this));
	RegisterAction(NewObject<UUDDefinePointTypeDealAction>(this));
	RegisterAction(NewObject<UUDAddInvokerDealAction>(this));
	RegisterAction(NewObject<UUDRemoveInvokerDealAction>(this));
	RegisterAction(NewObject<UUDAddTargetDealAction>(this));
	RegisterAction(NewObject<UUDRemoveTargetDealAction>(this));
	RegisterAction(NewObject<UUDReadyDealAction>(this));
	RegisterAction(NewObject<UUDNotReadyDealAction>(this));
	RegisterAction(NewObject<UUDPositiveVoteDealAction>(this));
	RegisterAction(NewObject<UUDNegativeVoteDealAction>(this));
	// Deal action finalizations
	RegisterAction(NewObject<UUDFinalizeItemsDealAction>(this));
	RegisterAction(NewObject<UUDAcceptFinalItemDealAction>(this));
	RegisterAction(NewObject<UUDDenyFinalItemDealAction>(this));
	RegisterAction(NewObject<UUDAlterFinalItemDealAction>(this));
	RegisterAction(NewObject<UUDSabotageFinalItemDealAction>(this));
	RegisterAction(NewObject<UUDExecuteAllActionsDealAction>(this));
}