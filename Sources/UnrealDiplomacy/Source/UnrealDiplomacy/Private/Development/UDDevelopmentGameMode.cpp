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

	// AI INIT IS DONE

	// FINALLY START GAME
	StartGame();

	// YES GAME IS RUNNING FOR PLAYERS TO DO THEIR DANCE MOVES

	UE_LOG(LogTemp, Log, TEXT("Testing simple simulation!"));
	// From perspective of human player as AI is partially automized.
	FUDActionData endTurnP1(UUDEndTurnAction::ActionTypeId, 1);
	WorldSimulation->ExecuteAction(endTurnP1);

	FUDActionData unconditionalGiftFrom1To2(UUDUnconditionalGiftAction::ActionTypeId, 1, 2, 69);
	WorldSimulation->ExecuteAction(unconditionalGiftFrom1To2);
	WorldSimulation->RevertAction();

	FUDActionData conditionalGiftFrom1To2(UUDGiftAction::ActionTypeId, 1, 2, 42);
	WorldSimulation->ExecuteAction(conditionalGiftFrom1To2);

	FIntPoint tile(1, 1);
	FUDActionData takeTile(UUDTakeTileAction::ActionTypeId, 1, 0, tile);
	WorldSimulation->ExecuteAction(takeTile);

	FUDActionData exploitTile = FUDActionData::Create(UUDExploitTileAction::ActionTypeId, 1, tile);
	WorldSimulation->ExecuteAction(exploitTile);

	FUDActionData permissionShare(UUDGrantExploitTilePermissionAction::ActionTypeId, 1, 2, tile);
	WorldSimulation->ExecuteAction(permissionShare);


	FIntPoint tileOther(2, 2);
	FUDActionData takeTileOther(UUDTakeTileAction::ActionTypeId, 1, 0, tileOther);
	WorldSimulation->ExecuteAction(takeTileOther);
	FUDActionData transferTileOther(UUDTransferTileAction::ActionTypeId, 1, 2, tileOther);
	WorldSimulation->ExecuteAction(transferTileOther);

	// Example of how this operates...
	// Test full turn cycle with incorrect end and undo attempts.
	//FUDActionData log(0, 1);
	// WorldSimulation->ExecuteAction(log);
	// moved to automatic execution 
		// FUDActionData endTurnP0(3, 0);
		// WorldSimulation->ExecuteAction(endTurnP0);
	// Test incorrect end turn.
	//FUDActionData endTurnP1(3, 1);
	//WorldSimulation->ExecuteAction(endTurnP1);
	//WorldSimulation->ExecuteAction(endTurnP1);
	//FUDActionData endTurnP2(3, 2);
	//WorldSimulation->ExecuteAction(endTurnP2);
	// Test how undo works over end turn.
	//WorldSimulation->ExecuteAction(endTurnP1);
	//WorldSimulation->RevertAction();
	//WorldSimulation->ExecuteAction(endTurnP1);
	//WorldSimulation->ExecuteAction(endTurnP2);
	//WorldSimulation->RevertAction();
	//WorldSimulation->RevertAction();
	//WorldSimulation->ExecuteAction(endTurnP2);
	//WorldSimulation->ExecuteAction(endTurnP1);
	//WorldSimulation->ExecuteAction(endTurnP2);
}

TObjectPtr<AUDSkirmishAIController> AUDDevelopmentGameMode::CreateAi()
{
	return GetWorld()->SpawnActor<AUDDevelopmentAIController>();
}