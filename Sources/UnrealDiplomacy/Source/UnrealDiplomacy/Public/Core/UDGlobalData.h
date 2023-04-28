// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGlobalData.generated.h"

/**
 * Collection of constants that are used as special values across the project.
 * Primary purpose of these constants is to define invalid values to avoid
 * ambiquity of whatever something is present & correct or not present.
 */
UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDGlobalData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * This Id is always considered invalid for all action operations.
	 */
	static const int32 InvalidActionId = -1;
	/**
	 * Amount of optional parameters required for actions. 
	 */
	static const int32 DefaultActionParameterCount = 0;
	/**
	 * This Id is always considered invalid for all state types.
	 */
	static const int32 InvalidStateId = -1;
	/**
	 * This Id is always reserved for Gaia for both player & state.
	 */
	static const int32 GaiaId = 0;
};

// DESC COMMENTS TO DELETE
/**
* START GAME
 * Start of game action that is called as action only once.
 * This is only due to fact that we are otherwise using only end turn action.
 * Which is for most purposes sufficient, as any player can check if they are currently playing
 * by observing their state. First player does not know this, so Gaia which takes the first turn
 * is awaiting for this to end her turn.
* ADD PLAYER
 * Notification for new Player/Ai being part of the game.
 * This allows new player to receive history and create list of all players.
 * Note: Invoker is always the new player, even, if he was added by someone esle AI by server.
 
 
 */