// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishGameMode.h"

AUDSkirmishGameMode::AUDSkirmishGameMode() 
{	
	GameStateClass = AUDSkirmishGameState::StaticClass();
	HUDClass = AUDSkirmishHUD::StaticClass();
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

TObjectPtr<AUDSkirmishGaiaAIController> AUDSkirmishGameMode::CreateServerPlayer()
{
	return GetWorld()->SpawnActor<AUDSkirmishGaiaAIController>();
}

void AUDSkirmishGameMode::RegisterPlayer(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("Setting up new Player."));
	// Cast obtained controller and save it.
	TObjectPtr<AUDSkirmishPlayerController> controller = Cast<AUDSkirmishPlayerController>(NewPlayer);
	PlayerControllers.Add(controller);

	AssignToSimulation(Cast<IUDControllerInterface>(controller), true);
}

void AUDSkirmishGameMode::RegisterAi()
{
	UE_LOG(LogTemp, Log, TEXT("Setting up new Ai."));
	// Create new controller and save it.
	TObjectPtr<AUDSkirmishAIController> controller = CreateAi();
	AiControllers.Add(controller);

	AssignToSimulation(Cast<IUDControllerInterface>(controller), true);

	controller->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(controller->GetControllerUniqueId()));
	GetCastGameState()->RegisterActionMaker(Cast<IUDActionHandlingInterface>(controller));
}

void AUDSkirmishGameMode::RegisterGaiaAi()
{
	UE_LOG(LogTemp, Log, TEXT("Setting up new Gaia."));
	// Create new controller and save it.
	GaiaController = CreateServerPlayer();

	// This is supposed to be first call, if this ever becomes out of order, it will cause
	// more issues as the initialization will not be able to handle a player that
	// initialized sooner then server class.
	AssignToSimulation(Cast<IUDControllerInterface>(GaiaController), false);

	GaiaController->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(GaiaController->GetControllerUniqueId()));
	GetCastGameState()->RegisterActionMaker(Cast<IUDActionHandlingInterface>(GaiaController));
}

void AUDSkirmishGameMode::AssignToSimulation(TObjectPtr<IUDControllerInterface> controller, bool isPlayerOrAi)
{
	// Always retrieve this before calling anything that might depend on it.
	TObjectPtr<AUDWorldSimulation> worldSim = GetWorldSimulation();
	// Define new controller ID.
	controller->SetControllerUniqueId(GetNextUniqueControllerId());
	UE_LOG(LogTemp, Log, TEXT("Finishing initialization of player with Id: %d"), controller->GetControllerUniqueId());
	// Register controller for WorldSimulation, so it has it's own unique representation in the server simulation.
	worldSim->CreateState(controller->GetControllerUniqueId(), isPlayerOrAi);
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
