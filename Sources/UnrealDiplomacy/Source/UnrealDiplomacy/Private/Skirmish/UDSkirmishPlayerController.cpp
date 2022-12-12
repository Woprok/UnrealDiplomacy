// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishPlayerController.h"

void AUDSkirmishPlayerController::OnRep_SetUniqueControllerId(const int32& oldId)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController: Synchronized Id from %d to %d."), oldId, UniqueControllerId);
	// if IsLocallyController() <-client?server-> GetLocalRole() == ROLE_Authority
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
	UE_LOG(LogTemp, Log, TEXT("Finalized initialization of UObjects and Actors for PlayerController."));
}

void AUDSkirmishPlayerController::ProcessReceivedAction(FUDActionData& actionData)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPlayerController: Execute from %d."), GetControllerUniqueId());
	// TODO maybe added variant for execute that does not check CanExecute ?
	//GetWorldSimulation()->ExecuteAction(actionData);
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