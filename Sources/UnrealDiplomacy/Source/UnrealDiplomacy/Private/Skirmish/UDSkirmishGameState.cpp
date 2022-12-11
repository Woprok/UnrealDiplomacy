// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGameState.h"
#include "Skirmish/UDSkirmishGameMode.h"

TObjectPtr<AUDSkirmishGameMode> AUDSkirmishGameState::GetCastGameMode()
{
	if (InternalCurrentGameMode.IsNull())
	{
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
	GetCastGameMode()->ProcessAction(clientData);
}

// Header part for this is automatically generated from RPC definition.
void AUDSkirmishGameState::MulticastSendActionToAllClients_Implementation(FUDActionData serverData)
{
	UE_LOG(LogTemp, Log, TEXT("UDSkirmishGameState Multicast as $d type"), GetNetMode());
	// TODO verify that this works in all possible conditions.
	TObjectPtr<APlayerController> controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//TObjectPtr<APlayerController> controller = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (controller.IsNull())
	{
		//controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UE_LOG(LogTemp, Log, TEXT("UDSkirmishGameState Multicast as $d type failed."), GetNetMode());
	}
	TObjectPtr<AUDSkirmishPlayerController> skirmishController = Cast<AUDSkirmishPlayerController>(controller);
	skirmishController->ProcessReceivedAction(serverData);
}