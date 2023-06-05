// Copyright Miroslav Valach

#include "Development/UDDevelopmentGameMode.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "Core/Simulation/UDActionInterface.h"

AUDDevelopmentGameMode::AUDDevelopmentGameMode() 
{
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Initialized."));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

void AUDDevelopmentGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TWeakObjectPtr<AUDWorldSimulation> sim = GetWorldSimulation();
	auto test = sim->GetFactionState(0);
	//if (IsValid(test))
	//{
	//	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Tick detected valid world."));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Tick detected invalid world."));
	//}
	//if (!GetWorldSimulation().IsValid())
	//{
	//	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Tick detected invalid world."));
	//}
}

void AUDDevelopmentGameMode:: BeginPlay()
{
	Super::BeginPlay();
	//PlayTest();
}

void AUDDevelopmentGameMode::PlayTest()
{
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Continuing execution after inherited BeginPlay..."));
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Spawning AI player."));
	//CreateAiPlayers(1);
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Starting game."));
	//StartGame();
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Testing few possible plays."));

	return;
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Testing finished."));
}