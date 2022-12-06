// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishGameMode.h"

AUDSkirmishGameMode::AUDSkirmishGameMode() 
{	
	GameStateClass = AUDGameState::StaticClass();
	HUDClass = AUDHUD::StaticClass();
	PlayerControllerClass = AUDSkirmishPlayerController::StaticClass();
	PlayerStateClass = AUDSkirmishPlayerState::StaticClass();
	UE_LOG(LogTemp, Log, TEXT("Defined static classes for SkirmishGameMode."));
}

void AUDSkirmishGameMode::Initialize()
{
	InternalWorldSimulation = GetWorld()->SpawnActor<AUDWorldSimulation>();
	GetWorldSimulation()->Initialize();
	RegisterAsListenerToWorldSimulation();
	UE_LOG(LogTemp, Log, TEXT("Finalized initialization of UObjects and Actors for GameMode."));
	RegisterGaiaAi();
	UE_LOG(LogTemp, Log, TEXT("Finalized creation of Gaia state for GameMode. Follows game log..."));
}

int32 AUDSkirmishGameMode::GetNextUniqueControllerId()
{
	UE_LOG(LogTemp, Log, TEXT("Current unset id(%d)."), NextUniqueControllerIdCount);
	return NextUniqueControllerIdCount++;
};

void AUDSkirmishGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	RegisterPlayer(NewPlayer);
}

void AUDSkirmishGameMode::RegisterPlayer(APlayerController* NewPlayer)
{

	TObjectPtr<AUDSkirmishPlayerController> controller = Cast<AUDSkirmishPlayerController>(NewPlayer);
	controller->SetControllerUniqueId(GetNextUniqueControllerId());
	PlayerControllers.Add(controller);
	UE_LOG(LogTemp, Log, TEXT("Finalizing setup of new Player with Id: %d"), controller->GetControllerUniqueId());
	UE_LOG(LogTemp, Log, TEXT("Retrieved Id(%d)."), controller->GetControllerUniqueId());
	AssignToSimulation(Cast<IUDControllerInterface>(controller));
}

void AUDSkirmishGameMode::CreateAiPlayers(int32 count)
{
	for (int32 i = 0; i < count; i++)
	{
		RegisterAi();
	}
}

TObjectPtr<AUDSkirmishAIController> AUDSkirmishGameMode::CreateAi()
{
	return GetWorld()->SpawnActor<AUDSkirmishAIController>();
}

void AUDSkirmishGameMode::RegisterAi()
{
	// TODO REDUCE AND MERGE AS MUCH AS POSSIBLE WITH PLAYER VERSION
	TObjectPtr<AUDSkirmishAIController> controller = CreateAi();
	controller->SetControllerUniqueId(GetNextUniqueControllerId());
	AiControllers.Add(controller);
	UE_LOG(LogTemp, Log, TEXT("Finalizing setup of new AI with Id: %d"), controller->GetControllerUniqueId());
	UE_LOG(LogTemp, Log, TEXT("Retrieved Id(%d)."), controller->GetControllerUniqueId());
	AssignToSimulation(Cast<IUDControllerInterface>(controller));
	controller->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(controller->GetControllerUniqueId()));
	GetCastGameState()->RegisterActionMaker(Cast<IUDActionHandlingInterface>(controller));
}

void AUDSkirmishGameMode::AssignToSimulation(TObjectPtr<IUDControllerInterface> playerOrAi)
{
	UE_LOG(LogTemp, Log, TEXT("Retrieved Interface Id(%d)."), playerOrAi->GetControllerUniqueId());
	GetWorldSimulation()->CreateState(playerOrAi->GetControllerUniqueId(), true);
}

void AUDSkirmishGameMode::RegisterGaiaAi()
{
	GaiaController = GetWorld()->SpawnActor<AUDSkirmishGaiaAIController>();
	// This is supposed to be first call, if this ever becomes out of order, it will cause
	// more issues as the initialization will not be able to handle a player that
	// initialized sooner then server class.
	GaiaController->SetControllerUniqueId(GetNextUniqueControllerId());
	GetWorldSimulation()->InitializeGaiaWorldState(GaiaController->GetControllerUniqueId());
	GaiaController->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(GaiaController->GetControllerUniqueId()));
	GetCastGameState()->RegisterActionMaker(Cast<IUDActionHandlingInterface>(GaiaController));
}

void AUDSkirmishGameMode::StartGame()
{
	// Test map gen action
	// TODO implement lobby setting passover
	FUDActionData mapGen(UUDCreateWorldMapAction::ActionTypeId, 4, FIntPoint(5,5));
	GetWorldSimulation()->ExecuteAction(mapGen);

	FUDActionData startGame(UUDStartGameAction::ActionTypeId);
	GetWorldSimulation()->ExecuteAction(startGame);
}

void AUDSkirmishGameMode::ProcessAction(FUDActionData& actionData)
{
	GetWorldSimulation()->ExecuteAction(actionData);
}

void AUDSkirmishGameMode::ActionExecutionFinished(FUDActionData& actionData)
{
	// GAMEMODE decides who will receive which action, based on expected requirements passed by simulation.
	
	// Gaia always receives action notification.
	GaiaController->OnActionExecuted(actionData);
	// TODO all

	// if all
	GetCastGameState()->MulticastSendActionToAllClients(actionData);
	// AI
	for (auto& controller : AiControllers)
	{
		controller->OnActionExecuted(actionData);
	}

	/*
	// TODO selection or single
	// else
	for (auto& controller : PlayerControllers)
	{
		// if is part
		controller->ClientcastReceiveActionFromServer(actionData);
	}
	// AI
	for (auto& controller : AiControllers)
	{ //if is in range
		controller->OnActionExecuted(actionData);
	}
	*/
}

void AUDSkirmishGameMode::RegisterAsListenerToWorldSimulation()
{
	GetWorldSimulation()->OnBroadcastActionExecutedDelegate.AddUObject(this, &AUDSkirmishGameMode::ActionExecutionFinished);
}
