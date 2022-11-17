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
	FUDActionData log(0, 0);
	WorldSimulation->ExecuteAction(log);
	FUDActionData endTurnP0(3, 0);
	WorldSimulation->ExecuteAction(endTurnP0);
	FUDActionData endTurnP1(3, 1);
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->ExecuteAction(endTurnP1);
	FUDActionData endTurnP2(3, 2);
	WorldSimulation->ExecuteAction(endTurnP2);
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->RevertAction();
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->RevertAction();
	WorldSimulation->RevertAction();
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->ExecuteAction(endTurnP2);
}

TObjectPtr<AUDSkirmishAIController> AUDDevelopmentGameMode::CreateAi()
{
	return GetWorld()->SpawnActor<AUDDevelopmentAIController>();
}