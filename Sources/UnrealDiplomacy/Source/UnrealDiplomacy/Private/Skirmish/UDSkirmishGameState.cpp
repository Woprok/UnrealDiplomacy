// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGameState.h"
#include "Skirmish/UDSkirmishGameMode.h"

TWeakObjectPtr<AUDSkirmishGameMode> AUDSkirmishGameState::GetCastGameMode()
{
	if (!InternalCurrentGameMode.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: New GameMode required."));
		InternalCurrentGameMode = Cast<AUDSkirmishGameMode>(AuthorityGameMode);
	}
	return InternalCurrentGameMode;
}

void AUDSkirmishGameState::RegisterActionMaker(TObjectPtr<IUDActionHandlingInterface> newListener)
{
	newListener->OnActionDecidedDelegate.BindUObject(this, &AUDSkirmishGameState::OnServerSendAction);
}

void AUDSkirmishGameState::OnServerSendAction(FUDActionData& clientData)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: Received Action for GameMode."));
	GetCastGameMode()->ProcessAction(clientData);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishGameState::MulticastSendActionToAllClients_Implementation(FUDActionData serverData)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: Multicast invoked by GameMode."));
	if (GetNetMode() < ENetMode::NM_Client)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: Multicast halted by being Server."));
		return;
	}

	// TODO verify that this works in all possible conditions.
	TObjectPtr<APlayerController> controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//TObjectPtr<APlayerController> controller = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (!IsValid(controller))
	{
		//controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UE_LOG(LogTemp, Log, TEXT("UDSkirmishGameState: Multicast failed to retrieve player controller."));
		return;
	}
	TObjectPtr<AUDSkirmishPlayerController> skirmishController = Cast<AUDSkirmishPlayerController>(controller);
	skirmishController->MulticastReceiveActionFromServer_Local(serverData);
}

void AUDSkirmishGameState::ResolveGetSyncData(int32 controllerId, int32 firstKnown)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: History requested by Player(%d)."), controllerId);
	GetCastGameMode()->SendPartialHistoricData(controllerId, firstKnown);
}