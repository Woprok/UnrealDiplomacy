// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGameMode.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/Actions/UDSystemActionWorldCreate.h"
#include "Core/Simulation/Actions/UDSystemActionGameStart.h"
#include "Core/UDControllerInterface.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDCommandData.h"
#include "Skirmish/UDSkirmishGameState.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishAIController.h"
#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Skirmish/UDSkirmishPlayerState.h"

#pragma region Constructors & Initialization

AUDSkirmishGameMode::AUDSkirmishGameMode()
{
	GameStateClass = AUDSkirmishGameState::StaticClass();
	HUDClass = AUDSkirmishHUD::StaticClass();
	PlayerControllerClass = AUDSkirmishPlayerController::StaticClass();
	PlayerStateClass = AUDSkirmishPlayerState::StaticClass();
	UE_LOG(LogTemp, Log, TEXT("Defined static classes for SkirmishGameMode."));
}

void AUDSkirmishGameMode::CreateWorldSimulation()
{
	InternalWorldSimulation = GetWorld()->SpawnActor<AUDWorldSimulation>();

	GetWorldSimulation()->Initialize();
	RegisterAsListenerToWorldSimulation();
	UE_LOG(LogTemp, Log, TEXT("Finalized initialization of UObjects and Actors for GameMode."));
	RegisterGaiaAi();
	UE_LOG(LogTemp, Log, TEXT("Finalized creation of Gaia state for GameMode. Follows game log..."));
}

TWeakObjectPtr<AUDWorldSimulation> AUDSkirmishGameMode::GetWorldSimulation()
{
	if (!InternalWorldSimulation.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: New simulation required."));
		CreateWorldSimulation();
	}
	return InternalWorldSimulation;
}

TWeakObjectPtr<AUDSkirmishGameState> AUDSkirmishGameMode::GetCastGameState()
{
	if (!InternalCurrentGameState.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: New GameState required."));
		InternalCurrentGameState = Cast<AUDSkirmishGameState>(GameState);
	}
	return InternalCurrentGameState;
}

void AUDSkirmishGameMode::RegisterAsListenerToWorldSimulation()
{
	GetWorldSimulation()->OnBroadcastVerifiedActionExecutedDelegate.AddUObject(this, &AUDSkirmishGameMode::ActionExecutionFinished);
}

#pragma endregion

#pragma region Login & Logout

void AUDSkirmishGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: New player joined."));
	Super::PostLogin(NewPlayer);
	RegisterPlayer(NewPlayer);
}

void AUDSkirmishGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player left."));
	Super::Logout(Exiting);
}

#pragma endregion

#pragma region Actions & Commands

void AUDSkirmishGameMode::ProcessAction(FUDActionData& action)
{
	// All actions are straight up passed to simulation as obtained from client.
	GetWorldSimulation()->ExecuteAction(action);
}

void AUDSkirmishGameMode::ProcessCommand(FUDCommandData& command)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: %s."), *command.ToString());
	switch (command.Command)
	{
	case EUDCommandType::None:
		break;
	case EUDCommandType::StartGame:
		OnStartGameCommand();
		break;
	default:
		unimplemented()
		break;
	}
}

void AUDSkirmishGameMode::OnStartGameCommand()
{
	// Executes sequence of commands required to start the game.
	// Sequence:
	// - Create AI players
	// - Create Map
	// - Start Game

	// TODO READ LOBBY CONFIG
	CreateAiPlayers(2);

	FUDActionData mapGen(UUDSystemActionWorldCreate::ActionTypeId, UUDGlobalData::GaiaId, { 4, 5, 5 });
	GetWorldSimulation()->ExecuteAction(mapGen);

	FUDActionData startGame(UUDSystemActionGameStart::ActionTypeId, UUDGlobalData::GaiaId);
	GetWorldSimulation()->ExecuteAction(startGame);
}

void AUDSkirmishGameMode::SendPartialHistoricData(int32 controllerId, int32 firstKnown)
{
	FUDActionArray actions = GetWorldSimulation()->GetHistoryUntil(firstKnown);
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Sending history to Player(%d)"), controllerId);
	for (auto& controller : PlayerControllers)
	{
		if (controller->GetControllerUniqueId() == controllerId)
		{
			controller->ClientcastInitialSyncReceiveFromServer(actions);
			break;
		}
	}
}

void AUDSkirmishGameMode::ActionExecutionFinished(FUDActionData& action)
{
	// Gaia always receives action notification.
	GaiaController->OnActionExecuted(action);
	// Players will receive action notification.
	GetCastGameState()->MulticastSendActionToAllClients(action);
	// AI will receive action notification.
	for (auto& controller : AiControllers)
	{
		controller->OnActionExecuted(action);
	}
}

#pragma endregion

#pragma region Controllers

#pragma endregion

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

	TScriptInterface<IUDControllerInterface> idController = controller.Get();

	AssignToSimulation(idController, true);
}

void AUDSkirmishGameMode::RegisterAi()
{
	UE_LOG(LogTemp, Log, TEXT("Setting up new Ai."));
	// Create new controller and save it.
	TWeakObjectPtr<AUDSkirmishAIController> controller = CreateAi();
	AiControllers.Add(controller);

	TScriptInterface<IUDControllerInterface> idController = controller.Get();

	AssignToSimulation(idController, true);

	controller->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(controller->GetControllerUniqueId()));

	TScriptInterface<IUDActionHandlingInterface> actionHandlingController = controller.Get();

	GetCastGameState()->RegisterActionMaker(actionHandlingController);
}

void AUDSkirmishGameMode::RegisterGaiaAi()
{
	UE_LOG(LogTemp, Log, TEXT("Setting up new Gaia."));
	// Create new controller and save it.
	GaiaController = CreateServerPlayer();

	TScriptInterface<IUDControllerInterface> idController = GaiaController.Get();
	
	// This is supposed to be first call, if this ever becomes out of order, it will cause
	// more issues as the initialization will not be able to handle a player that
	// initialized sooner then server class.
	AssignToSimulation(idController, false);

	GaiaController->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(GaiaController->GetControllerUniqueId()));

	TScriptInterface<IUDActionHandlingInterface> actionHandlingController = GaiaController.Get();
	
	GetCastGameState()->RegisterActionMaker(actionHandlingController);
}

void AUDSkirmishGameMode::AssignToSimulation(TScriptInterface<IUDControllerInterface>& controller, bool isPlayerOrAi)
{
	// Always retrieve this before calling anything that might depend on it.
	TWeakObjectPtr<AUDWorldSimulation> worldSim = GetWorldSimulation();
	// Define new controller ID.
	controller->SetControllerUniqueId(GetNextUniqueControllerId());
	UE_LOG(LogTemp, Log, TEXT("Finishing initialization of player with Id: %d"), controller->GetControllerUniqueId());
	// Register controller for WorldSimulation, so it has it's own unique representation in the server simulation.
	worldSim->CreateStateAndSynchronize(controller->GetControllerUniqueId(), isPlayerOrAi);
}
