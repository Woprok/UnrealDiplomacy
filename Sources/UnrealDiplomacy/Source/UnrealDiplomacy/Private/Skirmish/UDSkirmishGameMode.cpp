// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGameMode.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/Actions/UDSystemActionWorldCreate.h"
#include "Core/Simulation/Actions/UDSystemActionGameStart.h"

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

void AUDSkirmishGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: New player joined."));
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

TWeakObjectPtr<AUDSkirmishAIController> AUDSkirmishGameMode::CreateAi()
{
	return GetWorld()->SpawnActor<AUDSkirmishAIController>();
}

TWeakObjectPtr<AUDSkirmishGaiaAIController> AUDSkirmishGameMode::CreateServerPlayer()
{
	return GetWorld()->SpawnActor<AUDSkirmishGaiaAIController>();
}

void AUDSkirmishGameMode::RegisterPlayer(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("Setting up new Player."));
	// Cast obtained controller and save it.
	TWeakObjectPtr<AUDSkirmishPlayerController> controller = Cast<AUDSkirmishPlayerController>(NewPlayer);
	PlayerControllers.Add(controller);

	AssignToSimulation(Cast<IUDControllerInterface>(controller), true);
}

void AUDSkirmishGameMode::RegisterAi()
{
	UE_LOG(LogTemp, Log, TEXT("Setting up new Ai."));
	// Create new controller and save it.
	TWeakObjectPtr<AUDSkirmishAIController> controller = CreateAi();
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
	TWeakObjectPtr<AUDWorldSimulation> worldSim = GetWorldSimulation();
	// Define new controller ID.
	controller->SetControllerUniqueId(GetNextUniqueControllerId());
	UE_LOG(LogTemp, Log, TEXT("Finishing initialization of player with Id: %d"), controller->GetControllerUniqueId());
	// Register controller for WorldSimulation, so it has it's own unique representation in the server simulation.
	worldSim->CreateStateAndSynchronize(controller->GetControllerUniqueId(), isPlayerOrAi);
}

void AUDSkirmishGameMode::StartGame(FUDActionData& startAction)
{
	// TODO implement lobby functionality
	// TODO really do something about this
	// Start game requires mapGen to be done before it as everything after it depends on it.
	CreateAiPlayers(2);

	FUDActionData mapGen(UUDSystemActionWorldCreate::ActionTypeId, UUDGlobalData::GaiaId, { 4, 5, 5 });
	GetWorldSimulation()->ExecuteAction(mapGen);

	GetWorldSimulation()->ExecuteAction(startAction);
}

void AUDSkirmishGameMode::ProcessAction(FUDActionData& action)
{
	// Certain action require additional calls
	// TODO merge this additional calls to action execution.
	// This would require additional way for creating merged actions or
	// allowing actions to invoke additional actions.
	if (action.ActionTypeId == UUDSystemActionGameStart::ActionTypeId)
	{
		StartGame(action);
	}
	// Execute action as obtained.
	else 
	{
		GetWorldSimulation()->ExecuteAction(action);
	}
}

void AUDSkirmishGameMode::ActionExecutionFinished(FUDActionData& action)
{
	// GAMEMODE decides who will receive which action, based on expected requirements passed by simulation.
	
	// Gaia always receives action notification.
	GaiaController->OnActionExecuted(action);
	// TODO all

	// if all
	GetCastGameState()->MulticastSendActionToAllClients(action);
	// AI
	for (auto& controller : AiControllers)
	{
		controller->OnActionExecuted(action);
	}

	/*
	// TODO selection or single
	// else
	for (auto& controller : PlayerControllers)
	{
		// if is part
		controller->ClientcastReceiveActionFromServer(data);
	}
	// AI
	for (auto& controller : AiControllers)
	{ //if is in range
		controller->OnActionExecuted(data);
	}
	*/
}

void AUDSkirmishGameMode::SendPartialHistoricData(int32 controllerId, int32 firstKnown)
{
	// TODO add more strict checks for validity of request
	FUDActionArray actions = GetWorldSimulation()->GetHistoryUntil(firstKnown);
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Sending history to Player(%d)"), controllerId);
	// TODO swap this for map access, if we ever add a map
	// TODO swap this for array access, if we ever merge controllers to single array
	for (auto& controller : PlayerControllers)
	{
		if (controller->GetControllerUniqueId() == controllerId)
		{
			controller->ClientcastInitialSyncReceiveFromServer(actions);
			break;
		}
	}
}

void AUDSkirmishGameMode::RegisterAsListenerToWorldSimulation()
{
	GetWorldSimulation()->OnBroadcastVerifiedActionExecutedDelegate.AddUObject(this, &AUDSkirmishGameMode::ActionExecutionFinished);
}