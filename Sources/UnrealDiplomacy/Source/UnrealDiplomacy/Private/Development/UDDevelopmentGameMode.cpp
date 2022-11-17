// Copyright Miroslav Valach


#include "Development/UDDevelopmentGameMode.h"

AUDDevelopmentGameMode::AUDDevelopmentGameMode() 
{
	UE_LOG(LogTemp, Log, TEXT("Running DEVELOPMENT ONLY version of an SkirmishGameMode"));
}


void AUDDevelopmentGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("Testing environtment simulation!"));
	CreateAiPlayers(1);
	UE_LOG(LogTemp, Log, TEXT("Testing simple simulation!"));
	FUDActionData action(0, 0);
	WorldSimulation->ExecuteAction(action);
	/**
	Simulation->FinishTurn(0);
	Simulation->FinishTurn(1);
	Simulation->FinishTurn(1);
	Simulation->FinishTurn(2);
	Simulation->FinishTurn(0);*/
}

TObjectPtr<AUDSkirmishAIController> AUDDevelopmentGameMode::CreateAi()
{
	return GetWorld()->SpawnActor<AUDDevelopmentAIController>();
}