// Copyright Miroslav Valach
// TODO multithreading for data synchronization.

#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDCommandData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Tiles/UDSquareGrid.h"
#include "Skirmish/UDSkirmishGameState.h"
#include "Core/Simulation/UDWorldSimulation.h"

#pragma region Client-Server RPCs
void AUDSkirmishPlayerController::MulticastReceiveActionFromServer_Local(FUDActionData& action)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Multicast Receive."), GetControllerUniqueId());
	// Action received from server is always correct and will be executed on client.
	ReceiveAction(action);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ClientcastReceiveActionFromServer_Implementation(FUDActionData serverData)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController(%d) Clientcast Action Receive."), GetControllerUniqueId());
	// Action received from server is always correct and will be executed on client.
	ReceiveAction(serverData);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ServercastSendActionToServer_Implementation(FUDActionData clientData)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController(%d): Servercast Action Send."), GetControllerUniqueId());
	GetCastGameState()->OnServerSendAction(clientData);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ClientcastInitialSyncReceiveFromServer_Implementation(FUDActionArray actions)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController(%d) Clientcast Sync Receive."), GetControllerUniqueId());
	FinishDataSynchronization(actions);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ServercastInitialSyncRequestToServer_Implementation(int32 controllerId, int32 firstKnown)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController(%d): Servercast Sync Send."), GetControllerUniqueId());
	GetCastGameState()->ResolveGetSyncData(controllerId, firstKnown);
}
TWeakObjectPtr<AUDSkirmishGameState> AUDSkirmishPlayerController::GetCastGameState()
{
	if (!InternalCurrentGameState.IsValid())
	{
		InternalCurrentGameState = Cast<AUDSkirmishGameState>(GetWorld()->GetGameState());
	}
	return InternalCurrentGameState;
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ServercastSendCommandToServer_Implementation(FUDCommandData clientData)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController(%d): Servercast Command Send."), GetControllerUniqueId());
	GetCastGameState()->OnServerSendCommand(clientData);
}
#pragma endregion

#pragma region Synchronization

void AUDSkirmishPlayerController::StartClientSynchronization()
{
	// Always starts with data synchronization.
	// This should never be invoked twice as neither Server or Client are supposed to change the ControllerId again.
	StartJoinDataSynchronization();
}

void AUDSkirmishPlayerController::StartFactionChange()
{
	// This gets tricky...
	// We have two posibilities:
	// 1. fresh start
	// -> works easily by starting new sync
	StartJoinDataSynchronization();
	// 2. existing start
	//
	return;
}

void AUDSkirmishPlayerController::StartJoinDataSynchronization()
{
	if (SynchronizationState == EUDSynchronizationState::SynchronizingData)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Simultaneous attempts are not allowed for data synchronization."), GetControllerUniqueId());
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Starting data synchronization."), GetControllerUniqueId());
	SynchronizationState = EUDSynchronizationState::SynchronizingData;
	ServercastInitialSyncRequestToServer(GetControllerUniqueId(), GetFirstKnownActionId());
}

void AUDSkirmishPlayerController::AttemptSynchronization()
{
	int32 currentControllerId = GetControllerUniqueId();

	// Check if all ids are synchronized.
	if (!IsPropertySynchronized())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Missing at least one synchronization Id. Current FactionId(%d)"),
			currentControllerId, GetControlledFactionId());
		return;
	}
	// All ids are present.
	// Either is synchronization running or we can start it.
	if (SynchronizationState == EUDSynchronizationState::SynchronizingData)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Awaiting synchronization to finish."), currentControllerId);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Attempting synchronization to start."), currentControllerId);
		StartJoinDataSynchronization();
	}
}

int32 AUDSkirmishPlayerController::GetFirstKnownActionId()
{
	// Either we send that we have no valid Id or we send first valid Id.
	int32 firstKnownAction = UUDGlobalData::InvalidActionId;
	if (StoredActions.Num() > 0)
	{
		// This is first Id we currently hold, thus we don't need it again or anything after it.
		firstKnownAction = StoredActions[0].UniqueId;
	}

	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Returning known action id(%d)."), GetControllerUniqueId(), firstKnownAction);
	return firstKnownAction;
}

void AUDSkirmishPlayerController::ReceiveAction(FUDActionData& action)
{
	// Only synchronized client is able to execute actions that arrive by the main channel.
	if (SynchronizationState == EUDSynchronizationState::Synchronized)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Executing received action."), GetControllerUniqueId());
		RunAction(action);
	}
	// Non synchronized client has to wait for synchronization to finish.
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Checking synchronization state."), GetControllerUniqueId());
		StoreAction(action);
		AttemptSynchronization();
	}
}

void AUDSkirmishPlayerController::StoreAction(FUDActionData& action)
{
	// These should always arrive and be stored in order as we are using reliable RPCs.
	StoredActions.Add(action);
}

#pragma endregion

#pragma region Action Execution

void AUDSkirmishPlayerController::RunAction(FUDActionData& actionData)
{
	GetWorldSimulation()->OnlyExecuteAction(actionData);
}

void AUDSkirmishPlayerController::RunAllActions(TArray<FUDActionData>& actionArray)
{
	for (FUDActionData& action : actionArray)
	{
		GetWorldSimulation()->OnlyExecuteAction(action);
	}
}

void AUDSkirmishPlayerController::ChangeFaction()
{
	GetWorldSimulation()->CreatePrivatePlayerFaction(GetControlledFactionId());
	GetAdministrator()->SetOverseeingState(GetWorldSimulation()->GetFactionState(GetControlledFactionId()));
}

void AUDSkirmishPlayerController::InitializeSimulation()
{
	InternalWorldSimulation = GetWorld()->SpawnActor<AUDWorldSimulation>();
	GetWorldSimulation()->Initialize();
	GetWorldSimulation()->OnBroadcastActionAppliedDelegate.AddUObject(this, &AUDSkirmishPlayerController::OnWorldSimulationUpdated);
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Initialized with temporary Id."), GetControllerUniqueId());
}

void AUDSkirmishPlayerController::InitializeAdministrator()
{
	InternalPersonalAdministrator = NewObject<UUDActionAdministrator>();
	InternalPersonalAdministrator->OnUserActionRequestedDelegate.BindUObject(this, &AUDSkirmishPlayerController::OnUserActionRequested);
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Initialized personal administrator."), GetControllerUniqueId());
}

TWeakObjectPtr<AUDWorldSimulation> AUDSkirmishPlayerController::GetWorldSimulation()
{
	if (!InternalWorldSimulation.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController: New simulation required."));
		InitializeSimulation();
	}
	return InternalWorldSimulation;
}

UUDActionAdministrator* AUDSkirmishPlayerController::GetAdministrator()
{
	if (InternalPersonalAdministrator == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController: New administrator required."));
		InitializeAdministrator();
	}
	return InternalPersonalAdministrator;
}

#pragma endregion

void AUDSkirmishPlayerController::FinishDataSynchronization(FUDActionArray& actionArray)
{
	// Run all synchronizations
	ChangeFaction();
	RunAllActions(actionArray.Actions);
	RunAllActions(StoredActions);
	StoredActions.Empty();
	SynchronizationState = EUDSynchronizationState::Synchronized;

	// Finally notify everyone that this has finished.
	OnSynchronizationCompleted();
}

#pragma region Player Input & UI & Updates

void AUDSkirmishPlayerController::OnUserActionRequested(FUDActionData requestedAction)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Action choosed by user."), GetControllerUniqueId());
	ServercastSendActionToServer(requestedAction);
}

void AUDSkirmishPlayerController::OnWorldSimulationUpdated(FUDActionData& action)
{
	if (SynchronizationState == EUDSynchronizationState::Synchronized)
	{
		OnWorldStateUpdated();
	}
}

void AUDSkirmishPlayerController::OnSynchronizationCompleted()
{
	if (SynchronizationState == EUDSynchronizationState::Synchronized)
	{
		// This is required and done only here as it requires valid state to exists.
		InitializeGrid();
		OnSynchronizationFinished();
	}
}

#pragma endregion

#pragma region World

void AUDSkirmishPlayerController::InitializeGrid()
{
	if (SquareGrid.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): World Grid is already initialzied."), GetControllerUniqueId());
		return;
	}
	SquareGrid = GetWorld()->SpawnActor<AUDSquareGrid>(GridBlueprintClass, FVector(0, 0, 0), FRotator::ZeroRotator);
	SquareGrid->SetAuthority(GetAdministrator());
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): World Grid initialzied."), GetControllerUniqueId());
}

#pragma endregion