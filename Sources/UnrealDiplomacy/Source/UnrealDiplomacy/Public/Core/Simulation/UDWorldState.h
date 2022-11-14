// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
//ToDo is this required by anything at all ?
//#include "UObject/NoExportTypes.h"
#include "UDWorldState.generated.h"


/**
 * Holds the state of a world.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDWorldState : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Creates new instance of the world state.
	 * IsPlayerPerspectiveOnly defines if this state is supposed to hold all knowledge about the world.
	 */
	static TObjectPtr<UUDWorldState> CreateState(int32 playerId, bool isPlayerPerspectiveOnly);

	/**
	 * Id associated with a Player/Ai, that controls this simulation.
	 */
	int32 PerspectivePlayerId;
	/**
	 * State either belongs to specific Player/Ai or is global.
	 */
	bool IsPlayerPerspectiveOnly;
};
