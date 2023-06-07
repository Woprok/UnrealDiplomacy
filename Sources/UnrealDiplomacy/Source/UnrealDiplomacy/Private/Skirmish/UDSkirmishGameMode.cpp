// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGameMode.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/Actions/UDSystemActionWorldCreate.h"
#include "Core/Simulation/Actions/UDSystemActionGameStart.h"
#include "Core/Simulation/Actions/UDSystemActionLog.h"
#include "Core/Simulation/Actions/UDSystemActionPlayerRemove.h"
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
	NextUniqueControllerId = UUDGlobalData::FirstUseableControllerId;
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
	case EUDMatchState::Closed:
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player joined during the end of the match."));
		break;
	default:
		unimplemented();
		break;
	}

	FUDActionData playerSyncLog(UUDSystemActionLog::ActionTypeId, UUDGlobalData::GaiaFactionId);
	GetWorldSimulation()->CheckAndExecuteAction(playerSyncLog);
}

void AUDSkirmishGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player left."));
	TObjectPtr<AUDSkirmishPlayerController> player = Cast<AUDSkirmishPlayerController>(Exiting);

	if (player->IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Local Player Left -> Host Left."));
		MatchState = EUDMatchState::Closed;
	}

	OnPlayerLeaving(player);
	DestroyPlayer(player->GetControllerUniqueId());
	Super::Logout(Exiting);
}

#pragma endregion

#pragma region Actions & Commands

void AUDSkirmishGameMode::ProcessAction(FUDActionData& action)
{
	// All actions are straight up passed to simulation as obtained from client.
	GetWorldSimulation()->CheckAndExecuteAction(action);
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
	case EUDCommandType::HostClosedGame:
		OnCloseGameCommand();
		break;
	case EUDCommandType::PlayerLeftGame:
		// Ignored until we receive logout event.
		break;
	default:
		unimplemented();
		break;
	}
}

void AUDSkirmishGameMode::OnCloseGameCommand()
{
	MatchState = EUDMatchState::Closed;
	// TODO endgame evalutation...
}

void AUDSkirmishGameMode::OnStartGameCommand()
{
	// Executes sequence of commands required to start the game.
	// Sequence:
	// - Create AI players
	// - Create Map
	// - Start Game
	MatchState = EUDMatchState::Match;
	// TODO READ LOBBY CONFIG
	CreateAiPlayers(2);

	FUDActionData mapGen(UUDSystemActionWorldCreate::ActionTypeId, UUDGlobalData::GaiaFactionId, { 4, 5, 5 });
	GetWorldSimulation()->CheckAndExecuteAction(mapGen);

	FUDActionData startGame(UUDSystemActionGameStart::ActionTypeId, UUDGlobalData::GaiaFactionId);
	GetWorldSimulation()->CheckAndExecuteAction(startGame);
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

void AUDSkirmishGameMode::DestroyPlayer(int32 controllerUniqueId)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Removing controller (%d) from total (%d)."), 
		controllerUniqueId, PlayerControllers.Num());
	// Removes the player controller.
	// This is using RemoveAll variant so we can just filter it with predicate.
	// There should always be only one reference.
	PlayerControllers.RemoveAll(
		[&controllerUniqueId](const TWeakObjectPtr<AUDSkirmishPlayerController>& item) 
		{ 
			return item->GetControllerUniqueId() == controllerUniqueId; 
		}
	);
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Removed controller (%d) kept only (%d)."),
		controllerUniqueId, PlayerControllers.Num());
}

void AUDSkirmishGameMode::OnPlayerLeaving(TObjectPtr<AUDSkirmishPlayerController> existingPlayer)
{
	if (!GetWorldSimulation()->IsPlayerFaction(existingPlayer->GetControlledFactionId()))
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player was not playing for playable faction."));
		return;
	}

	FUDActionData removeFaction(UUDSystemActionLog::ActionTypeId, UUDGlobalData::GaiaFactionId);
	FUDActionData aiTakeOverAction(UUDSystemActionLog::ActionTypeId, UUDGlobalData::GaiaFactionId);

	switch (MatchState)
	{
	case EUDMatchState::Lobby:
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Player escaped from lobby."));
		// TODO replace it with Faction remove / ignore instead of replacing it with AI.
		RegisterSubstiteAi(existingPlayer->GetControlledFactionId());
		GetWorldSimulation()->CheckAndExecuteAction(removeFaction);
		break;
	case EUDMatchState::Match:
		RegisterSubstiteAi(existingPlayer->GetControlledFactionId());
		GetWorldSimulation()->CheckAndExecuteAction(aiTakeOverAction);
		break;
	case EUDMatchState::Closed:
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Game is being closed and AITakeover is disabled."));
		break;
	default:
		unimplemented();
		break;
	}
}

void AUDSkirmishGameMode::DefineUniqueControllerId(TScriptInterface<IUDControllerInterface>& controller)
{
	// Define new controller ID.
	controller->SetControllerUniqueId(NextUniqueControllerId++);
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

void AUDSkirmishGameMode::RegisterSubstiteAi(int32 claimableFactionId)
{
	// Create new controller and save it.
	TWeakObjectPtr<AUDSkirmishAIController> aiController = CreateAiPlayer();

	if (aiController == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGameMode: Failed to set up Ai player, changing state to Closed."));
		MatchState = EUDMatchState::Closed;
		return;
	}

	AiControllers.Add(aiController);
	// Assign Controller Id
	TScriptInterface<IUDControllerInterface> controller = aiController.Get();
	DefineUniqueControllerId(controller);

	controller->SetControlledFactionId(claimableFactionId);

	// Bind remaining AI functionality.
	aiController->SetSimulatedStateAccess(GetWorldSimulation()->GetFactionState(controller->GetControlledFactionId()));
	TScriptInterface<IUDActionHandlingInterface> actionHandlingController = aiController.Get();
	GetCastGameState()->RegisterActionMaker(actionHandlingController);
}

void AUDSkirmishGameMode::RegisterPlayer(TObjectPtr<AUDSkirmishPlayerController> playerController)
{
	// Save to collection.
	PlayerControllers.Add(playerController);
	// Assign Controller Id
	TScriptInterface<IUDControllerInterface> controller = playerController.Get();
	DefineUniqueControllerId(controller);

	int32 factionId = GetWorldSimulation()->CreatePlayerFaction();
	controller->SetControlledFactionId(factionId);
	GetWorldSimulation()->AnnounceFaction(factionId);
}

void AUDSkirmishGameMode::RegisterObserver(TObjectPtr<AUDSkirmishPlayerController> playerController)
{
	// Save to collection.
	PlayerControllers.Add(playerController);
	// Assign Controller Id
	TScriptInterface<IUDControllerInterface> controller = playerController.Get();
	DefineUniqueControllerId(controller);

	int32 factionId = GetWorldSimulation()->CreateObserverFaction();
	controller->SetControlledFactionId(factionId);
	GetWorldSimulation()->AnnounceFaction(factionId);
}

void AUDSkirmishGameMode::RegisterAi()
{
	// Create new controller and save it.
	TWeakObjectPtr<AUDSkirmishAIController> aiController = CreateAiPlayer();
	AiControllers.Add(aiController);
	// Assign Controller Id
	TScriptInterface<IUDControllerInterface> controller = aiController.Get();
	DefineUniqueControllerId(controller);

	int32 factionId = GetWorldSimulation()->CreatePlayerFaction();
	controller->SetControlledFactionId(factionId);
	GetWorldSimulation()->AnnounceFaction(factionId);

	// Bind remaining AI functionality.
	aiController->SetSimulatedStateAccess(GetWorldSimulation()->GetFactionState(controller->GetControlledFactionId()));
	TScriptInterface<IUDActionHandlingInterface> actionHandlingController = aiController.Get();
	GetCastGameState()->RegisterActionMaker(actionHandlingController);
}

void AUDSkirmishGameMode::RegisterGaiaAi()
{
	// Create new controller and save it.
	GaiaController = CreateServerPlayer();
	// Assign Controller Id that is expected by global definition.
	TScriptInterface<IUDControllerInterface> controller = GaiaController.Get();
	controller->SetControllerUniqueId(UUDGlobalData::GaiaControllerId);

	int32 factionId = GetWorldSimulation()->CreateGaiaFaction();
	controller->SetControlledFactionId(factionId);

	// Bind remaining AI functionality.
	GaiaController->SetSimulatedStateAccess(GetWorldSimulation()->GetFactionState(GaiaController->GetControlledFactionId()));
	TScriptInterface<IUDActionHandlingInterface> actionHandlingController = GaiaController.Get();
	GetCastGameState()->RegisterActionMaker(actionHandlingController);
}

#pragma endregion