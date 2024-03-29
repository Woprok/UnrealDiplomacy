// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/UDGlobalData.h"

TObjectPtr<UUDTileState> UUDTileState::CreateState(int32 x, int32 y)
{
	TObjectPtr<UUDTileState> newState = NewObject<UUDTileState>();
	newState->Position = FIntPoint(x, y);
	newState->OwnerUniqueId = UUDGlobalData::GaiaFactionId;
	newState->Type = 0;
	newState->Name = FString::Format(TEXT("Province X:{0} Y:{1}"), 
		{ FString::Printf(TEXT("%*d"), 2, x), FString::Printf(TEXT("%*d"), 2, y) }
	);
	newState->ResourceType = UUDGlobalData::InvalidResourceId;
	newState->ResourceStockpile = 0;
	return newState;
}

TObjectPtr<UUDTileState> UUDTileState::Duplicate(TObjectPtr<UUDTileState> existingState)
{
	TObjectPtr<UUDTileState> newState = NewObject<UUDTileState>();
	newState->Position = FIntPoint(existingState->Position.X, existingState->Position.Y);
	newState->OwnerUniqueId = existingState->OwnerUniqueId;
	newState->Type = existingState->Type;
	newState->Name = FString(existingState->Name);
	newState->ResourceType = existingState->ResourceType;
	newState->ResourceStockpile = existingState->ResourceStockpile;
	return MoveTempIfPossible(newState);
}

TObjectPtr<UUDMapState> UUDMapState::CreateState(int32 seed, int32 sizeOfX, int32 sizeOfY, int32 requiredFactionSpace)
{
	TObjectPtr<UUDMapState> newState = NewObject<UUDMapState>();
	newState->MapSeed = seed;
	newState->MapSizeOfX = sizeOfX;
	newState->MapSizeOfY = sizeOfY;
	newState->RequiredFactionSpace = requiredFactionSpace;

	// TODO remove this hack
	while (requiredFactionSpace > newState->MapSizeOfX * newState->MapSizeOfY)
	{
		newState->MapSizeOfX++;
		newState->MapSizeOfY++;
	}

	return newState;
}

TObjectPtr<UUDMapState> UUDMapState::Duplicate(TObjectPtr<UUDMapState> existingState)
{
	// Self duplication
	TObjectPtr<UUDMapState> newState = NewObject<UUDMapState>();
	newState->MapSeed = existingState->MapSeed;
	newState->MapSizeOfX = existingState->MapSizeOfX;
	newState->MapSizeOfY = existingState->MapSizeOfY;

	// Recursive duplicate on tiles
	newState->Tiles.SetNumZeroed(existingState->MapSizeOfX * existingState->MapSizeOfY);

	for (int32 xy = 0; xy < existingState->MapSizeOfX * existingState->MapSizeOfY; xy++)
	{
		newState->Tiles[xy] = UUDTileState::Duplicate(existingState->Tiles[xy]);
	}

	return newState;
}

TObjectPtr<UUDFactionState> UUDFactionState::CreateState(int32 factionId)
{
	TObjectPtr<UUDFactionState> newState = NewObject<UUDFactionState>();
	newState->PlayerUniqueId = factionId;
	newState->PendingDecisions.Empty(0);
	newState->AccessibleStratagemOptions.Empty(0);
	newState->PickedStratagemOptions.Empty(0);
	newState->Name = FString::Format(TEXT("Faction {0}"), { factionId });
	newState->Controller = EUDFactionController::Error;
	newState->Resources.Empty();
	// Resource initialization was moved to action.
	// Thus see SystemActionPlayerAdd for the resource initialization.
	
	return newState;
}

TObjectPtr<UUDDealState> UUDDealState::CreateState(int32 dealId, int32 ownerId)
{
	TObjectPtr<UUDDealState> newState = NewObject<UUDDealState>();
	newState->DealSimulationState = EUDDealSimulationState::CreateAndAssemble;
	newState->DealSimulationResult = EUDDealSimulationResult::Opened;
	newState->UniqueDealId = dealId;
	newState->OwnerUniqueId = ownerId;
	newState->Participants.Empty(0);
	newState->BlockedParticipants.Empty(0);
	newState->Points.Empty(0);
	newState->ChatHistory.Empty(0);
	newState->IsReadyPlayerList.Empty(0);
	newState->PositiveVotePlayerList.Empty(0);
	newState->Name = FString::Format(TEXT("Deal {0}"), { dealId });
	newState->CanBeExecuted = false;
	return newState;
}

TObjectPtr<UUDDiscussionItem> UUDDiscussionItem::CreateState(int32 ownerId)
{
	TObjectPtr<UUDDiscussionItem> newState = NewObject<UUDDiscussionItem>();
	newState->EditorId = ownerId;
	newState->ActionId = -1;
	newState->Type = EUDPointType::Proposal;
	newState->IsIgnored = false;
	newState->Consequencies.Empty(0);
	newState->Invoker = - 1;
	newState->ValueParameters.Empty(0);
	newState->TextParameter.Empty(0);
	return newState;
}

TObjectPtr<UUDWorldState> UUDWorldState::CreateState(int32 playerId, EUDWorldPerspective perspectiveType)
{
	TObjectPtr<UUDWorldState> newState = NewObject<UUDWorldState>();
	newState->Perspective = perspectiveType;
	newState->FactionPerspective = playerId;
	newState->TurnData = FUDTurnState();
	newState->Settings = FUDSettings();
	newState->Settings.MapWidth = 5;
	newState->Settings.MapHeight = 5;
	newState->Settings.StratagemPoints = 5;
	newState->ImperialThrone = FUDThroneState(UUDGlobalData::GaiaFactionId);
	return newState;
}