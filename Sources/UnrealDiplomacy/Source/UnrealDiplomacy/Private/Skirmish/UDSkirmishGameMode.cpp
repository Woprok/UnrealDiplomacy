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
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Defined static classes for SkirmishGameMode."));
}

void AUDSkirmishGameMode::CreateWorldSimulation()
{
	InternalWorldSimulation = GetWorld()->SpawnActor<AUDWorldSimulation>();

	GetWorldSimulation()->Initialize();
	RegisterAsListenerToWorldSimulation();
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Finalized initialization of UObjects and Actors for GameMode."));
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Setting up new Gaia player."));
	RegisterGaiaAi();
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Finalized creation of Gaia state for GameMode. Follows game log..."));
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
	TObjectPtr<AUDSkirmishPlayerController> player = Cast<AUDSkirmishPlayerController>(NewPlayer);
	switch (MatchState)
	{
	case EUDMatchState::Lobby:
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player joined as player."));
		RegisterPlayer(player);
		break;
	case EUDMatchState::Match:
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player joined as observer."));
		RegisterObserver(player);
		break;
	default:
		unimplemented();
		break;
	}
}

void AUDSkirmishGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player left."));
	Super::Logout(Exiting);
	OnAITakeOver(Exiting);
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
		unimplemented();
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
	EUDMatchState MatchState = EUDMatchState::Match;
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

void AUDSkirmishGameMode::DefineUniqueControllerId(TScriptInterface<IUDControllerInterface>& controller)
{
	// Define new controller ID.
	controller->SetControllerUniqueId(NextUniqueControllerIdCount++);
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Finishing initialization of controller with Id: %d"), controller->GetControllerUniqueId());
}

void AUDSkirmishGameMode::CreateAiPlayers(int32 count)
{
	for (int32 i = 0; i < count; i++)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Setting up %d. Ai player."), i);
		RegisterAi();
	}
}

TWeakObjectPtr<AUDSkirmishAIController> AUDSkirmishGameMode::CreateAiPlayer()
{
	return GetWorld()->SpawnActor<AUDSkirmishAIController>();
}

TWeakObjectPtr<AUDSkirmishGaiaAIController> AUDSkirmishGameMode::CreateServerPlayer()
{
	return GetWorld()->SpawnActor<AUDSkirmishGaiaAIController>();
}

void AUDSkirmishGameMode::RegisterPlayer(TObjectPtr<AUDSkirmishPlayerController> controller)
{
	// Save to collection.
	PlayerControllers.Add(controller);

	// Assign Faction state to player
	TScriptInterface<IUDControllerInterface> idController = controller.Get();
	AssignToSimulation(idController, true);
}

void AUDSkirmishGameMode::RegisterObserver(TObjectPtr<AUDSkirmishPlayerController> controller)
{
	// Save to collection.
	PlayerControllers.Add(controller);

}

void AUDSkirmishGameMode::RegisterAi()
{
	// Create new controller and save it.
	TWeakObjectPtr<AUDSkirmishAIController> controller = CreateAiPlayer();
	AiControllers.Add(controller);

	// 
	TScriptInterface<IUDControllerInterface> idController = controller.Get();
	AssignToSimulation(idController, true);
	controller->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(controller->GetControllerUniqueId()));

	// Bind AI action invokes.
	TScriptInterface<IUDActionHandlingInterface> actionHandlingController = controller.Get();
	GetCastGameState()->RegisterActionMaker(actionHandlingController);
}

void AUDSkirmishGameMode::RegisterGaiaAi()
{
	// Create new controller and save it.
	GaiaController = CreateServerPlayer();

	TScriptInterface<IUDControllerInterface> idController = GaiaController.Get();
	AssignToSimulation(idController, false);
	GaiaController->SetSimulatedStateAccess(GetWorldSimulation()->GetSpecificState(GaiaController->GetControllerUniqueId()));

	// Bind AI action invokes.
	TScriptInterface<IUDActionHandlingInterface> actionHandlingController = GaiaController.Get();
	GetCastGameState()->RegisterActionMaker(actionHandlingController);
}

void AUDSkirmishGameMode::AssignToSimulation(TScriptInterface<IUDControllerInterface>& controller, bool isPlayerOrAi)
{
	// Always retrieve this before calling anything that might depend on it.
	TWeakObjectPtr<AUDWorldSimulation> worldSim = GetWorldSimulation();


	// Register controller for WorldSimulation, so it has it's own unique representation in the server simulation.
	worldSim->CreateStateAndSynchronize(controller->GetControllerUniqueId(), isPlayerOrAi);
}

#pragma endregion