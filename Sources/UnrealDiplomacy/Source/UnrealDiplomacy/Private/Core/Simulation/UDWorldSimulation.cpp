// Copyright Miroslav Valach
// TODO fix missing check for observer and gaia, when someone attempts to add another one into the state
// expected behaviour is to never call initialization for observer and gaia twice.
// at the moment announce is executed twice. This was introduced when creation and sync were separated.

#include "Core/Simulation/UDWorldSimulation.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionManager.h"
#include "Core/UDSettingManager.h"
#include "Core/Simulation/UDWorldArbiter.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDStratagemUseManager.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDActionHandlingInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"

void AUDWorldSimulation::Initialize(TWeakObjectPtr<UUDSettingManager> settingManager, TWeakObjectPtr<UUDActionManager> actionManager)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Initializing."));
	NextUniqueFactionId = UUDGlobalData::FirstUseableFactionId;
	NextUniqueActionId = UUDGlobalData::FirstUseableActionId;
	ActionManager = actionManager;
	SettingManager = settingManager;
	StratagemUseManager = ActionManager->GetStratagemUseManager();
	
	Arbiter = NewObject<UUDWorldArbiter>(this);
	Arbiter->SetModifierManager(ActionManager->GetModifierManager());
	Arbiter->SetResourceManager(ActionManager->GetResourceManager());
	Arbiter->SetSettingManager(SettingManager);
}

#pragma region Assigns of controllers to specific state.

bool AUDWorldSimulation::IsPlayerFaction(int32 factionId)
{
	// This is just shortcut at the moment for checking against Gaia & Observer.
	bool notGaia = factionId != UUDGlobalData::GaiaFactionId;
	bool notObserver = factionId != UUDGlobalData::ObserverFactionId;
	bool isPlayable = factionId >= UUDGlobalData::FirstUseableFactionId;
	return notGaia && notObserver && isPlayable;
}

int32 AUDWorldSimulation::CreateGaiaFaction()
{
	if (States.Contains(UUDGlobalData::GaiaFactionId))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Gaia Faction already exists under Id %d."), UUDGlobalData::GaiaFactionId);
		return UUDGlobalData::GaiaFactionId;
	}

	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering new Gaia Faction %d."), UUDGlobalData::GaiaFactionId);
	CreateSynchronizedState(UUDGlobalData::GaiaFactionId, EUDWorldPerspective::Gaia);
	return UUDGlobalData::GaiaFactionId;
}

int32 AUDWorldSimulation::CreateObserverFaction()
{
	if (States.Contains(UUDGlobalData::ObserverFactionId))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Observer Faction already exists under Id %d."), UUDGlobalData::ObserverFactionId);
		return UUDGlobalData::ObserverFactionId;
	}

	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering new Observer Faction %d."), UUDGlobalData::ObserverFactionId);
	CreateSynchronizedState(UUDGlobalData::ObserverFactionId, EUDWorldPerspective::Observer);
	return UUDGlobalData::ObserverFactionId;
}

int32 AUDWorldSimulation::CreatePlayerFaction()
{
	int32 newFactionId = GetNewFactionId();
	if (States.Contains(newFactionId))
	{
		// This should never happen, so just do the most obvious thing -> crash!
		ensureMsgf(false, TEXT("AUDWorldSimulation: Encountered same Id(%d) registration!"), newFactionId);
		return UUDGlobalData::InvalidFactionId;
	}

	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Registering new Player Faction %d."), newFactionId);
	CreateSynchronizedState(newFactionId, EUDWorldPerspective::Faction);
	return newFactionId;
}

void AUDWorldSimulation::CreatePrivatePlayerFaction(int32 factionId)
{
	EUDWorldPerspective perspective = EUDWorldPerspective::Faction;
	switch (factionId)
	{
	case UUDGlobalData::GaiaFactionId:
		perspective = EUDWorldPerspective::Gaia;
		break;
	case UUDGlobalData::ObserverFactionId:
		perspective = EUDWorldPerspective::Observer;
		break;
	default:
		perspective = EUDWorldPerspective::Faction;
		break;
	}
	if (States.Contains(factionId))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Specified local Player Faction already exists under Id %d."), factionId);
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Created local Player Faction %d."), factionId);
	CreateNewState(factionId, perspective);
}

int32 AUDWorldSimulation::GetNewFactionId()
{
	return NextUniqueFactionId++;
}

void AUDWorldSimulation::CreateSynchronizedState(int32 factionId, EUDWorldPerspective perspective)
{
	CreateNewState(factionId, perspective);
	SynchronizeNewFactionState(States[factionId]);
}

void AUDWorldSimulation::AnnounceFaction(int32 factionId)
{
	CreateFactionCreationEvent(factionId);
}

void AUDWorldSimulation::CreateNewState(int32 factionId, EUDWorldPerspective perspective)
{
	TObjectPtr<UUDWorldState> newState = UUDWorldState::CreateState(factionId, EUDWorldPerspective::Faction);
	States.Add(factionId, newState);
}

void AUDWorldSimulation::SynchronizeNewFactionState(TObjectPtr<UUDWorldState> newFactionState)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Synchrinizing new Player."));
	for (auto& action : ExecutionHistory)
	{
		ActionManager->GetAction(action.ActionTypeId)->Execute(action, newFactionState);
	}
}

void AUDWorldSimulation::CreateFactionCreationEvent(int32 factionId)
{
	FUDActionData joinPlayer(UUDSystemActionPlayerAdd::ActionTypeId, factionId);
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Calling join action for player id(%d)."), factionId);
	CheckAndExecuteAction(joinPlayer);
}
#pragma endregion

#pragma region Data
TObjectPtr<UUDWorldState> AUDWorldSimulation::GetFactionState(int32 factionId)
{
	if (States.Contains(factionId))
	{
		return States[factionId];
	}
	return nullptr;
};
#pragma endregion

#pragma region Actions

bool AUDWorldSimulation::HasValidActionId(int32 currentId)
{
	return currentId != UUDGlobalData::InvalidActionId;
}

int32 AUDWorldSimulation::GetNewActionId()
{
	return NextUniqueActionId++;
}

void AUDWorldSimulation::OnlyExecuteAction(FUDActionData& trustworthyAction)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: OnlyExecuteAction UID(%d)."), trustworthyAction.UniqueId);
	// Run action on the all available states without any checks.
	for (auto& pair : States)
	{
		ActionManager->GetAction(trustworthyAction.ActionTypeId)->Execute(trustworthyAction, pair.Value);
	}
	OnBroadcastActionAppliedDelegate.Broadcast(trustworthyAction);
}

void AUDWorldSimulation::CheckAndExecuteAction(FUDActionData& newAction, bool inheritedBypass)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: CheckAndExecuteAction Started."));

	// Retrieves Executor for this action from the Action Manager.
	TScriptInterface<IUDActionInterface> actionExecutor = ActionManager->GetAction(newAction.ActionTypeId);
	TObjectPtr<UUDWorldState> gaiaFactionState = GetFactionState(UUDGlobalData::GaiaFactionId);

	// Check if action can be executed with the retrieved executor.
	if (!actionExecutor->CanExecute(newAction, gaiaFactionState))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Executor halted for action type id(%d) due to executor type id(%d)."),
			newAction.ActionTypeId, actionExecutor->GetId());
		return;
	}

	// Check stratagem rules, if bypass is not true
	if (!inheritedBypass)
	{
		StratagemUseManager->DoMagic();
	}

	// Prepare action.
	AssignActionIds(newAction);
	CreateActionBackup(newAction, actionExecutor, gaiaFactionState);
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Executing Action UID(%d)."), newAction.UniqueId);
	
	// Incorporate into the history.
	ExecutionHistory.Add(newAction);
	
	// Updated all current states with this action.
	OnlyExecuteAction(newAction);
	
	// Notifies everyone about the verified action.
	OnBroadcastVerifiedActionExecutedDelegate.Broadcast(newAction);
	
	// Continue execution if it has any continuations.
	if (actionExecutor->HasContinuations())
	{
		bool isThisBypassing = actionExecutor->IsBypassingStratagemRequirements() || inheritedBypass;
		RunActionContinuations(newAction, actionExecutor, gaiaFactionState, isThisBypassing);
	}

	// Check endgame
	PostActionStateCheck(actionExecutor->GetId(), gaiaFactionState);
	
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: CheckAndExecuteAction Ended for Action UID(%d)."), 
		newAction.UniqueId);
}

#pragma endregion

#pragma region CheckAndExecute Helpers
void AUDWorldSimulation::AssignActionIds(FUDActionData& newAction)
{
	// Each action has UniqueId.
	if (!HasValidActionId(newAction.UniqueId))
	{
		newAction.UniqueId = GetNewActionId();
	}
}
void AUDWorldSimulation::CreateActionBackup(FUDActionData& newAction,
	TScriptInterface<IUDActionInterface>& actionExecutor,
	TObjectPtr<UUDWorldState>& gaiaFactionState)
{
	// Backup
	if (actionExecutor->IsBackupRequired())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Backup by UID(%d)."), newAction.UniqueId);
		actionExecutor->Backup(newAction, gaiaFactionState);
	}
}
void AUDWorldSimulation::RunActionContinuations(FUDActionData& newAction,
	TScriptInterface<IUDActionInterface>& actionExecutor,
	TObjectPtr<UUDWorldState>& gaiaFactionState,
	bool inheritedBypass)
{
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Composite Action UID(%d) Started."), newAction.UniqueId);
	TArray<FUDActionData> subactions = actionExecutor->GetContinuations(newAction, gaiaFactionState);
	for (auto& action : subactions)
	{
		CheckAndExecuteAction(action, inheritedBypass);
	}
	UE_LOG(LogTemp, Log, TEXT("AUDWorldSimulation: Composite Action UID(%d) Finished."), newAction.UniqueId);
}
void AUDWorldSimulation::PostActionStateCheck(int32 actionExecutorId,
	TObjectPtr<UUDWorldState>& gaiaFactionState)
{
	// This is basically required to check after every action due to current structure.
	// In return we are only checking actions we think can end the game.
	if (Arbiter->OnActionExecutionFinished(actionExecutorId, gaiaFactionState))
	{
		for (auto& action : Arbiter->EndGame())
		{
			CheckAndExecuteAction(action);
		}
	}
}
#pragma endregion

#pragma region Server Sync
FUDActionArray AUDWorldSimulation::GetHistoryUntil(int32 historyPoint)
{
	FUDActionArray newHistory;
	newHistory.Actions.SetNumZeroed(0);
	if (historyPoint == UUDGlobalData::InvalidActionId)
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
#pragma endregion

#pragma region Revert

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

#pragma endregion

int32 AUDWorldSimulation::GetDesiredAiCount()
{
	return States[UUDGlobalData::GaiaFactionId]->Settings.AICount;
}