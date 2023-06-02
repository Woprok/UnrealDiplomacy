// Copyright Miroslav Valach
// TODO if any synchronization issues are present, then multicast might not work as intended.

#include "Skirmish/UDSkirmishGameState.h"
#include "Skirmish/UDSkirmishGameMode.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionHandlingInterface.h"
#include "Kismet/GameplayStatics.h"

TWeakObjectPtr<AUDSkirmishGameMode> AUDSkirmishGameState::GetCastGameMode()
{
	if (!InternalCurrentGameMode.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: New GameMode required."));
		InternalCurrentGameMode = Cast<AUDSkirmishGameMode>(AuthorityGameMode);
	}
	return InternalCurrentGameMode;
}

void AUDSkirmishGameState::RegisterActionMaker(TScriptInterface<IUDActionHandlingInterface>& newListener)
{
	newListener->OnActionDecidedDelegate.BindUObject(this, &AUDSkirmishGameState::OnServerSendAction);
}

void AUDSkirmishGameState::OnServerSendAction(FUDActionData clientData)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: Received Action for GameMode."));
	GetCastGameMode()->ProcessAction(clientData);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishGameState::MulticastSendActionToAllClients_Implementation(FUDActionData serverData)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: Multicast invoked by GameMode. AUID(%d of %d)"), serverData.UniqueId, serverData.InvokerFactionId);

	TObjectPtr<APlayerController> controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(controller))
	{
		UE_LOG(LogTemp, Log, TEXT("UDSkirmishGameState: Multicast failed to retrieve player controller."));
		return;
	}
	TObjectPtr<AUDSkirmishPlayerController> skirmishController = Cast<AUDSkirmishPlayerController>(controller);
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: Multicast can attempt to execute on controller(%d)."), skirmishController->GetControllerUniqueId());

	skirmishController->MulticastReceiveActionFromServer_Local(serverData);
}

void AUDSkirmishGameState::ResolveGetSyncData(int32 controllerId, int32 firstKnown)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameState: History requested by Player(%d)."), controllerId);
	GetCastGameMode()->SendPartialHistoricData(controllerId, firstKnown);
}