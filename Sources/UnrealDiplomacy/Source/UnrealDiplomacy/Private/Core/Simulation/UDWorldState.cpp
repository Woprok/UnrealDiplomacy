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
	newState->ResourceType = UD_RESOURCE_FOOD_ID;
	newState->ResourceStored = 0;
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
	newState->ResourceStored = existingState->ResourceStored;
	return MoveTempIfPossible(newState);
}

TObjectPtr<UUDMapState> UUDMapState::CreateState(int32 seed, int32 sizeOfX, int32 sizeOfY)
{
	TObjectPtr<UUDMapState> newState = NewObject<UUDMapState>();
	newState->MapSeed = seed;
	newState->MapSizeOfX = sizeOfX;
	newState->MapSizeOfY = sizeOfY;
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
	newState->PendingRequests.Empty(0);
	newState->StratagemOptions.Empty(0);
	newState->Name = FString::Format(TEXT("Faction {0}"), { factionId });
	newState->Controller = EUDFactionController::Error;
	newState->Resources.Empty();
	// TODO replace this with resource manager initialization.
	newState->Resources.Add(UD_RESOURCE_REPUTATION_ID, 0);
	newState->Resources.Add(UD_RESOURCE_GOLD_ID, 50);
	newState->Resources.Add(UD_RESOURCE_FOOD_ID, 200);
	newState->Resources.Add(UD_RESOURCE_MATERIALS_ID, 100);
	newState->Resources.Add(UD_RESOURCE_LUXURIES_ID, 0);
	
	return newState;
}

TObjectPtr<UUDDealState> UUDDealState::CreateState(int32 dealId, int32 ownerId)
{
	TObjectPtr<UUDDealState> newState = NewObject<UUDDealState>();
	newState->DealSimulationState = EUDDealSimulationState::CreatingDraft;
	newState->DealSimulationResult = EUDDealSimulationResult::Opened;
	newState->UniqueDealId = dealId;
	newState->OwnerUniqueId = ownerId;
	newState->Participants.Empty(0);
	newState->BlockedParticipants.Empty(0);
	newState->Points.Empty(0);
	newState->ChatHistory.Empty(0);
	newState->IsReadyPlayerList.Empty(0);
	newState->PositiveVotePlayerList.Empty(0);
	return newState;
}

TObjectPtr<UUDDiscussionItem> UUDDiscussionItem::CreateState(int32 ownerId)
{
	TObjectPtr<UUDDiscussionItem> newState = NewObject<UUDDiscussionItem>();
	newState->EditorId = ownerId;
	newState->ActionId = 0;
	newState->Type = EUDPointType::Proposal;
	newState->IsIgnored = false;
	newState->Consequencies.Empty(0);
	newState->Invokers.Empty(0);
	newState->Targets.Empty(0);
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
	newState->Settings.StratagemPoints = 3;
	newState->ImperialThrone = FUDThroneState(UUDGlobalData::GaiaFactionId);
	return newState;
}