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

	// Test full turn cycle with incorrect end and undo attempts.
	FUDActionData endTurnP0(3, 0);
	WorldSimulation->ExecuteAction(endTurnP0);
	// Test incorrect end turn.
	FUDActionData endTurnP1(3, 1);
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->ExecuteAction(endTurnP1);
	FUDActionData endTurnP2(3, 2);
	WorldSimulation->ExecuteAction(endTurnP2);
	// Test how undo works over end turn.
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->RevertAction();
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->RevertAction();
	WorldSimulation->RevertAction();
	WorldSimulation->ExecuteAction(endTurnP0);
	WorldSimulation->ExecuteAction(endTurnP2);

	// Test unconditional gift
	FUDActionData generateIncome(100, 0);
	WorldSimulation->ExecuteAction(generateIncome);
	FUDActionData unconditionalGiftFrom0To1(1000, 0, 1, 100);
	WorldSimulation->ExecuteAction(unconditionalGiftFrom0To1);
	WorldSimulation->RevertAction();
}

TObjectPtr<AUDSkirmishAIController> AUDDevelopmentGameMode::CreateAi()
{
	return GetWorld()->SpawnActor<AUDDevelopmentAIController>();
}