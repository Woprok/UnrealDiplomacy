// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishPlayerController.h"

void AUDSkirmishPlayerController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDSkirmishPlayerController::GetControllerUniqueId()
{
	return UniqueControllerId;
}

void AUDSkirmishPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	LocalSimulation = GetWorld()->SpawnActor<AUDWorldSimulation>();
	LocalSimulation->Initialize();
	//LoadSkirmishAction();
	UE_LOG(LogTemp, Log, TEXT("Finalized initialization of UObjects and Actors for GameMode."));
	//RegisterGaiaAi();
	UE_LOG(LogTemp, Log, TEXT("Finalized creation of Gaia state for GameMode. Follows game log..."));
}

void AUDSkirmishPlayerController::ProcessReceivedAction(FUDActionData& actionData)
{
	// TODO maybe added variant for execute that does not check CanExecute ?
	LocalSimulation->ExecuteAction(actionData);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ClientcastReceiveActionFromServer_Implementation(FUDActionData serverData)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController Clientcast Receive as $d type"), GetNetMode());

	// Action received from server is always correct and will be executed on client.
	ProcessReceivedAction(serverData);
	// TODO call event that will notify UI, so the new change is propagated to UI.
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishPlayerController::ServercastSendActionToServer_Implementation(FUDActionData clientData)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishPlayerController Servercast as $d type"), GetNetMode());
	GetCastGameState()->OnServerSendAction(clientData);
}
