// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldState.h"

TObjectPtr<UUDTileState> UUDTileState::CreateState(int32 x, int32 y)
{
	TObjectPtr<UUDTileState> newState = NewObject<UUDTileState>();
	newState->Position = FIntPoint(x, y);
	newState->OwnerUniqueId = UUDWorldState::GaiaWorldStateId;
	return newState;
}

TObjectPtr<UUDTileState> UUDTileState::Duplicate(TObjectPtr<UUDTileState> existingState)
{
	TObjectPtr<UUDTileState> newState = NewObject<UUDTileState>();
	newState->Position = FIntPoint(existingState->Position.X, existingState->Position.Y);
	newState->OwnerUniqueId = existingState->OwnerUniqueId;
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

TObjectPtr<UUDNationState> UUDNationState::CreateState(int32 playerId)
{
	TObjectPtr<UUDNationState> newState = NewObject<UUDNationState>();
	newState->PlayerUniqueId = playerId;
	newState->PendingRequests.SetNum(0);
	return newState;
}

TObjectPtr<UUDWorldState> UUDWorldState::CreateState(int32 playerId, bool isPlayerPerspectiveOnly)
{
	TObjectPtr<UUDWorldState> newState = NewObject<UUDWorldState>();
	newState->IsPlayerPerspectiveOnly = isPlayerPerspectiveOnly;
	newState->PerspectivePlayerId = playerId;
	return newState;
}
