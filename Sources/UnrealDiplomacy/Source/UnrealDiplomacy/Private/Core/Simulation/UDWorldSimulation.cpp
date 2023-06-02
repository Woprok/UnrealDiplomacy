// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldSimulation.h"
#include "Core/Simulation/UDActionManager.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"

void AUDWorldSimulation::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Initializing."));
	ActionManager = NewObject<UUDActionManager>(this);
	Arbiter = NewObject<UUDWorldArbiter>(this);

	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Actions."));
	ActionManager->Initialize();
}

int32 AUDWorldSimulation::CreateNewState(EUDWorldPerspective stateType)
{
	int32 newStateId = GetNewStateId();

	// if observer get existing or create observer

	// if gaia do gaia

	// if faction do faction

	// if nation do nation...

	return newStateId;
}

void AUDWorldSimulation::CreateState(int32 playerId, bool isPlayerOrAi)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Creating new State."));
	if (States.Contains(playerId))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Duplicate initialization of player state for player with id(%d)."), playerId);
		return;
	}

	if (!isPlayerOrAi && playerId == UUDGlobalData::GaiaId)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Gaia as Id(%d)."), playerId);
	}
	else if (isPlayerOrAi && playerId != UUDGlobalData::GaiaId)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering Player or Ai as Id(%d)."), playerId);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Invalid combination of Id(%d) and isPlayerOrAi."), playerId);
		return;
	}
	TObjectPtr<UUDWorldState> newState;
	if (isPlayerOrAi)
		newState = UUDWorldState::CreateState(playerId, EUDWorldPerspective::Faction);
	else
		newState = UUDWorldState::CreateState(playerId, EUDWorldPerspective::Everything);

	States.Add(playerId, newState);
}

void AUDWorldSimulation::CreateStateAndSynchronize(int32 playerId, bool isPlayerOrAi)
{
	CreateState(playerId, isPlayerOrAi);
	SynchronizeNewPlayerState(States[playerId]);
	// After that push new synchronize action to all, including new joined player.
	FUDActionData joinPlayer(UUDSystemActionPlayerAdd::ActionTypeId, States[playerId]->FactionPerspective);
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Calling join action for player id(%d)."), States[playerId]->FactionPerspective);
	ExecuteAction(joinPlayer);
}

void AUDWorldSimulation::SynchronizeNewPlayerState(TObjectPtr<UUDWorldState> newState)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Synchrinizing new Player."));
	// New player state must be synchronzied from old action list first.
	// This directly executes action over the supplied state.
	for (auto& action : ExecutionHistory)
	{
		ActionManager->GetAction(action.ActionTypeId)->Execute(action, newState);
	}
}

void AUDWorldSimulation::NaiveExecuteAction(FUDActionData& trustworthyAction)
{
	// This expects action data that passed through ExecuteAction and thus are valid and complete.
	// This avoids checks over full state, thus allowing execution even if full state is not present.
	// This also ignores simulation call for broadcast. Maybe this is actually useful ?
	// TODO move these comments and verify their trustworthyness.
	for (auto& pair : States)
	{
		ActionManager->GetAction(trustworthyAction.ActionTypeId)->Execute(trustworthyAction, pair.Value);
	}
	OnBroadcastActionAppliedDelegate.Broadcast(trustworthyAction);
}

void AUDWorldSimulation::ExecuteAction(FUDActionData& newAction)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Executing Action."));

	auto actionExecutor = ActionManager->GetAction(newAction.ActionTypeId);

	//if (!Actions.Contains(newAction.ActionTypeId))
	//{
	//	// Blocked execution of non-existing action.
	//	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action executor for action id(%d) is not defined."), newAction.ActionTypeId);
	//	return;
	//}
	//// Obtained executor for this action.
	//auto& actionExecutor = Actions[newAction.ActionTypeId];

	if (!actionExecutor->CanExecute(newAction, GetSpecificState(UUDGlobalData::GaiaId)))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action executor was halted for action id(%d) due to executor id(%d)."),
			newAction.ActionTypeId, actionExecutor->GetId());
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

	if (actionExecutor->IsBackupRequired())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Backup by UID(%d)."), newAction.UniqueId);
		actionExecutor->Backup(newAction, GetSpecificState(UUDGlobalData::GaiaId));
	}

	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Action UID(%d)."), newAction.UniqueId);

	// Saved for future reference
	ExecutionHistory.Add(newAction);
	// Updated all current states with this action.
	NaiveExecuteAction(newAction);
	// Notifies everyone about the action.
	OnBroadcastVerifiedActionExecutedDelegate.Broadcast(newAction);
	// Continue notifying about the subsequent actions...
	if (actionExecutor->HasContinuations())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Composite Action UID(%d) Started."), newAction.UniqueId);
		TArray<FUDActionData> subactions = actionExecutor->GetContinuations(newAction, GetSpecificState(UUDGlobalData::GaiaId));
		for (auto& action : subactions)
		{
			ExecuteAction(action);
		}
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Composite Action UID(%d) Finished."), newAction.UniqueId);
	}
	// This is basically required to check after every action due to current structure.
	// In return we are only checking actions we think can end the game.
	if (Arbiter->OnActionExecutionFinished(actionExecutor->GetId(), GetSpecificState(UUDGlobalData::GaiaId)))
	{
		for (auto& action : Arbiter->EndGame())
		{
			ExecuteAction(action);
		}
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
		ActionManager->GetAction(oldAction.ActionTypeId)->Revert(oldAction, pair.Value);
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