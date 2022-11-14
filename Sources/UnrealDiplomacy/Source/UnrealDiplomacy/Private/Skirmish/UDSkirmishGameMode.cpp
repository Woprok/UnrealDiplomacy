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
	WorldSimulation->InitializeGaiaWorldState();
	UE_LOG(LogTemp, Log, TEXT("Finalized initialization of UObjects and Actors for GameMode."));
}

int32 AUDSkirmishGameMode::GetNextUniqueControllerId()
{
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
	AssignToSimulation(controller);
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
	AssignToSimulation(controller);
}

void AUDSkirmishGameMode::AssignToSimulation(TObjectPtr<IUDControllerInterface> playerOrAi)
{
	WorldSimulation->CreateState(playerOrAi->GetControllerUniqueId(), true);
}