// Copyright Miroslav Valach


#include "Core/Simulation/UDWorldState.h"

TObjectPtr<UUDNationState> UUDNationState::CreateState(int32 playerId)
{
	TObjectPtr<UUDNationState> newState = NewObject<UUDNationState>();
	newState->PlayerUniqueId = playerId;
	return newState;
}

TObjectPtr<UUDWorldState> UUDWorldState::CreateState(int32 playerId, bool isPlayerPerspectiveOnly)
{
	TObjectPtr<UUDWorldState> newState = NewObject<UUDWorldState>();
	newState->IsPlayerPerspectiveOnly = isPlayerPerspectiveOnly;
	newState->PerspectivePlayerId = playerId;
	return newState;
}