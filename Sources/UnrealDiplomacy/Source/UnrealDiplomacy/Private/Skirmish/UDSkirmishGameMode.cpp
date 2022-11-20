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

void AUDSkirmishGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	WorldSimulation = GetWorld()->SpawnActor<AUDWorldSimulation>();
	LoadSkirmishAction();
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
	TObjectPtr<AUDSkirmishAIController> controller = CreateAi();
	controller->SetControllerUniqueId(GetNextUniqueControllerId());
	AiControllers.Add(controller);
	UE_LOG(LogTemp, Log, TEXT("Finalizing setup of new AI with Id: %d"), controller->GetControllerUniqueId());
	UE_LOG(LogTemp, Log, TEXT("Retrieved Id(%d)."), controller->GetControllerUniqueId());
	WorldSimulation->RegisterActionMaker(Cast<IUDActionHandlingInterface>(controller));
	AssignToSimulation(Cast<IUDControllerInterface>(controller));
}

void AUDSkirmishGameMode::AssignToSimulation(TObjectPtr<IUDControllerInterface> playerOrAi)
{
	UE_LOG(LogTemp, Log, TEXT("Retrieved Interface Id(%d)."), playerOrAi->GetControllerUniqueId());
	WorldSimulation->CreateState(playerOrAi->GetControllerUniqueId(), true);
}

void AUDSkirmishGameMode::LoadSkirmishAction()
{
	// Basics 0+
	WorldSimulation->RegisterAction(NewObject<UUDLogAction>());
	WorldSimulation->RegisterAction(NewObject<UUDAddPlayerAction>());
	WorldSimulation->RegisterAction(NewObject<UUDEndTurnAction>());
	// Gaia 100+
	WorldSimulation->RegisterAction(NewObject<UUDGenerateIncomeAction>());
	// Player 1000+
	WorldSimulation->RegisterAction(NewObject<UUDUnconditionalGiftAction>());
}

void AUDSkirmishGameMode::RegisterGaiaAi()
{
	GaiaController = GetWorld()->SpawnActor<AUDSkirmishGaiaAIController>();
	// This is supposed to be first call, if this ever becomes out of order, it will cause
	// more issues as the initialization will not be able to handle a player that
	// initialized sooner then server class.
	GaiaController->SetControllerUniqueId(GetNextUniqueControllerId());
	WorldSimulation->RegisterActionMaker(Cast<IUDActionHandlingInterface>(GaiaController));
	WorldSimulation->InitializeGaiaWorldState(GaiaController->GetControllerUniqueId());
}