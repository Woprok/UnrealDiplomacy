// Copyright Miroslav Valach

#include "Development/UDDevelopmentGameMode.h"
#include "Core/Simulation/UDWorldSimulation.h"
#include "Core/Simulation/UDActionInterface.h"

AUDDevelopmentGameMode::AUDDevelopmentGameMode() 
{
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Initialized."));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUDDevelopmentGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetWorldSimulation().IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Tick detected invalid world."));
	}
}

void AUDDevelopmentGameMode:: BeginPlay()
{
	Super::BeginPlay();
	PlayTest();
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
	// From perspective of human player as AI is partially automized.
	//FUDActionData endTurnP1(UUDEndTurnAction::ActionTypeId, 1);
	//GetCastGameState()->OnServerSendAction(endTurnP1);
	//FUDActionData unconditionalGiftFrom1To2(UUDUnconditionalGiftAction::ActionTypeId, 1, 2, 69);
	//GetCastGameState()->OnServerSendAction(unconditionalGiftFrom1To2);
	//GetWorldSimulation()->RevertAction();
	//
	//FUDActionData conditionalGiftFrom1To2(UUDGiftAction::ActionTypeId, 1, 2, 42);
	//GetCastGameState()->OnServerSendAction(conditionalGiftFrom1To2);
	//
	//FIntPoint tile(1, 1);
	//FUDActionData takeTile(UUDTakeTileAction::ActionTypeId, 1, 0, tile);
	//GetCastGameState()->OnServerSendAction(takeTile);
	//
	//FUDActionData exploitTile = FUDActionData::Create(UUDExploitTileAction::ActionTypeId, 1, tile);
	//GetCastGameState()->OnServerSendAction(exploitTile);
	//
	//FUDActionData permissionShare(UUDGrantExploitTilePermissionAction::ActionTypeId, 1, 2, tile);
	//GetCastGameState()->OnServerSendAction(permissionShare);
	//
	//
	//FIntPoint tileOther(2, 2);
	//FUDActionData takeTileOther(UUDTakeTileAction::ActionTypeId, 1, 0, tileOther);
	//GetCastGameState()->OnServerSendAction(takeTileOther);
	//
	//FUDActionData transferTileOther(UUDTransferTileAction::ActionTypeId, 1, 2, tileOther);
	//GetCastGameState()->OnServerSendAction(transferTileOther);

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
	UE_LOG(LogTemp, Log, TEXT("AUDDevelopmentGameMode: Testing finished."));
}