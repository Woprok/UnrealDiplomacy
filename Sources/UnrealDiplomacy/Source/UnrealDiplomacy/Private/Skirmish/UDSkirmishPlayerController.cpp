// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishPlayerController.h"

void AUDSkirmishPlayerController::OnRep_SetUniqueControllerId(const int32& oldId)
{
	if (GetNetMode() < NM_Client)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Synchronized Id from original %d. [SERVER]"), GetControllerUniqueId(), oldId);
		// This should not fire on server...
		return;
	}

	if (IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Synchronized Id from original %d. [CLIENT]"), GetControllerUniqueId(), oldId);
		// This will never fire on client that runs server...
		StartSynchronization();
	}
}

void AUDSkirmishPlayerController::StartSynchronization()
{
	if (IsSyncInProgress)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Repeated attempts are not allowed for synchronization."), GetControllerUniqueId());
		return;
	}
	IsSyncInProgress = true;

	int32 firstKnown = 0;
	if (TemporaryActionHolder.Num() > 0)
	{
		// This is first Id we currently hold, thus we don't need it again or anything after it.
		firstKnown = TemporaryActionHolder[0].UniqueId;
	}
	ServercastInitialSyncRequestToServer(GetControllerUniqueId(), firstKnown);
}

void AUDSkirmishPlayerController::FinishSynchronization(FUDActionArray& actionArray)
{
	GetWorldSimulation()->CreateState(GetControllerUniqueId(), true);
	// TODO add some verification for size of received and saved.
	// This actually does the synchronization part
	// TODO maybe move this to simulation as well ?
	for (FUDActionData& action : actionArray.Actions)
	{
		GetWorldSimulation()->NaiveExecuteAction(action);
	}

	for (FUDActionData& action : TemporaryActionHolder)
	{
		GetWorldSimulation()->NaiveExecuteAction(action);
	}

	IsSynchronized = true;
	TemporaryActionHolder.Reset(0);
}

void AUDSkirmishPlayerController::VerifySyncInProgress()
{
	// Default value is 0 and that is always equal to Gaia, which is never correct for client.
	// TODO maybe rework this to something that can't be a problem, like ever...
	if (UniqueControllerId != UUDWorldState::GaiaWorldStateId && IsSyncInProgress)
	{
		// Has Id and is already in progress of receiving necessary history.
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Awaiting synchronization."), GetControllerUniqueId());
	}
	else if (UniqueControllerId != UUDWorldState::GaiaWorldStateId && !IsSyncInProgress)
	{
		// Has Id and is not already in prgoress of receiving necessary history. 
		// This by default should happen only if client is running server.
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Starting synchronization."), GetControllerUniqueId());
		StartSynchronization();
	}
	else
	{
		// Does not have Id and thus can not start yet.
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Missing synchronization Id."), GetControllerUniqueId());
	}
}

void AUDSkirmishPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUDSkirmishPlayerController, UniqueControllerId);
}

void AUDSkirmishPlayerController::Initialize()
{
	InternalWorldSimulation = GetWorld()->SpawnActor<AUDWorldSimulation>();

	GetWorldSimulation()->Initialize();
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Initialized with temporary Id."), GetControllerUniqueId());
}

void AUDSkirmishPlayerController::ProcessAction(FUDActionData& actionData)
{
	// TODO call event that will notify UI, so the new change is propagated to UI.
	// TODO maybe added variant for execute that does not check CanExecute ?

	// Only synchronized client is able to execute actions that arrive by the main channel.
	if (IsSynchronized)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Executing."), GetControllerUniqueId());
		GetWorldSimulation()->NaiveExecuteAction(actionData);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Awaiting sync."), GetControllerUniqueId());
		SaveActionUntilSynchronization(actionData);
		// Attempt to sync...
		VerifySyncInProgress();
	}
}

void AUDSkirmishPlayerController::SaveActionUntilSynchronization(FUDActionData& actionData)
{
	// These should arrive in order as we are using reliable RPCs.
	TemporaryActionHolder.Add(actionData);
}

void AUDSkirmishPlayerController::MulticastReceiveActionFromServer_Local(FUDActionData& actionData)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController(%d): Multicast Receive."), GetControllerUniqueId());
	ProcessAction(actionData);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ClientcastReceiveActionFromServer_Implementation(FUDActionData serverData)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController(%d) Clientcast Action Receive."), GetControllerUniqueId());
	// Action received from server is always correct and will be executed on client.
	ProcessAction(serverData);
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
	FinishSynchronization(actions);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ServercastInitialSyncRequestToServer_Implementation(int32 controllerId, int32 firstKnown)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController(%d): Servercast Sync Send."), GetControllerUniqueId());
	GetCastGameState()->ResolveGetSyncData(controllerId, firstKnown);
}