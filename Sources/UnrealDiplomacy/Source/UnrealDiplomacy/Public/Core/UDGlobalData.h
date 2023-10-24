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
	/** This Id is always used by first action in action list / history. */
	static const int32 FirstUseableActionId = 0;
	/** This is first Id that can be assigned to player / AI Faction. */
	static const int32 FirstUseableFactionId = 2;
	/** This is first Id that can be assigned to player / AI State. */
	static const int32 FirstUseableStateId = 2;
	/** This is first Id that can be assigned to player / AI Controller. */
	static const int32 FirstUseableControllerId = 1;

	/** This Id is always considered invalid for all action operations. */
	static const int32 InvalidActionId = -1;
	/** This Id is always considered invalid for all faction operations. */
	static const int32 InvalidFactionId = -1;
	/** This Id is always considered invalid for all state operations. */
	static const int32 InvalidStateId = -1;
	/** This Id is always considered invalid for all controller operations. */
	static const int32 InvalidControllerId = -1;

	/** This Id is always reserved for Gaia / Server Faction. */
	static const int32 GaiaFactionId = 0;     
	/** This Id is always reserved for Gaia / Server State. */
	static const int32 GaiaStateId = 0;	      
	/** This Id is always reserved for Gaia / Server Controller. */
	static const int32 GaiaControllerId = 0;

	/** This Id is always reserved for Observer Faction. */
	static const int32 ObserverFactionId = 1;
	/** This Id is always reserved for Observer State. */
	static const int32 ObserverStateId = 1;

	/** This Id is always considered invalid for all tile operations. */
	static const int32 InvalidTilePositionX = -1;
	static const int32 InvalidTilePositionY = -1;
	/** This Index is always considered invalid for all parameter index operations. */
	static const int32 InvalidParameterArrayIndex = -1;
	/** This Index is always considered invalid for all index operations. */
	static const int32 InvalidArrayIndex = -1;
	/** This Id is always considered invalid for all resource operations. */
	static const int32 InvalidResourceId = -1;
	/** This Id is always considered invalid for all modifier operations. */
	static const int32 InvalidModifierId = -1;
	/** This Id is always considered invalid for all deal operations. */
	static const int32 InvalidDealId = -1;
	/** This Id is always considered invalid for all deal point operations. */
	static const int32 InvalidDealPointId = -1;
public:
	// Default values that are expected to exists or be used.

	/**
	 * Amount of optional parameters required for actions.
	 */
	static const int32 DefaultActionParameterCount = 0;
	/**
	 * Amount of optional parameters required for modifiers.
	 */
	static const int32 DefaultModifierParameterCount = 0;
	/**
	 * Default level name that should be same as default level.
	 */
	static const FString DefaultLevelName()
	{
		return TEXT("Menu");
	}
	/**
	 * Option required for session travel.
	 */
	static const FString ListenOption()
	{
		return TEXT("listen");
	}
};

/**
 * Few optional configs, that change behaviour of systems.
 */
UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDGlobalGameConfig : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static const bool IsDirectExecutionEnabled = true;
};

// Following types are wrappers around otherwise engine types or values.

/**
 * Wrapper for all supported window modes.
 */
UENUM(BlueprintType)
enum class EUDWindowModeType : uint8
{
	/**
	 *
	 */
	Fullscreen,
	/**
	 *
	 */
	Borderless,
	/**
	 *
	 */
	Windowed,
};

/**
 * Custom type that can hold most of the application settings with non-engine references.
 */
USTRUCT(BlueprintType)
struct FUDApplicationSettings
{
	GENERATED_BODY()
public:
	FUDApplicationSettings() { }
	FUDApplicationSettings(FIntPoint resolution, EUDWindowModeType windowMode)
		: Resolution(resolution), WindowMode(windowMode) { }
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Resolution;
	UPROPERTY(BlueprintReadOnly)
	EUDWindowModeType WindowMode;
	static FUDApplicationSettings GetDefault();
};

UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDApplicationConverters : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Converts from engine enum to module enum.
	 */
	static EUDWindowModeType ToWindowModeType(EWindowMode::Type windowMode);
	/**
	 * Converts from module enum to engine enum.
	 */
	static EWindowMode::Type FromWindowModeType(EUDWindowModeType windowMode);
	/**
	 * Converts from engine enum to bool.
	 */
	static bool FromCheckBoxState(ECheckBoxState checkedState);
	/**
	 * Converts from bool to engine enum.
	 */
	static ECheckBoxState ToCheckBoxState(bool isChecked);
};

// DESC COMMENTS TO CHECK, CONSIDER, DELETE
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
* DEALS
 * Deal actions between any amount of players.
 * Core part of the game. Uses 10000-19999 for all subsequent actions.
 * Creates new deal.
 * Player that created the deal is considered a participant immediately.
 * Deals do not have an owner and are stored as part of world state.
 
 */

 // TODO LIST
 // TODO extend by adding more functions that do verify & check restriction
 // UUDUnconditionalGiftAction is missing condition (action.InvokerFactionId != action.TargetPlayerId)
 // TODO CanExecute overloads that check if the action is still queued in PendingRequests.
 // TODO Create action class that has function RemovePendingTargetRequest as default implementation.
 // TODO reverting confirm or reject does not result in same action, if the action would be modified, e.g.
 // only part of value is accepted as gift the original action would not be restorable. In case of adding
 // new option that makes partial accept, confirm for that action should be separate and it should extend parameter
 // count with the original and changed value.
 // TODO update modifiers to use predicates to minimize code
 // TODO consider that action like responses or deal creation could be using sequence of actions e.g. composite
 // this would allow them to do only one thing, while allowing additional effects to take place via consequent actions.
 // TODO reevaluate UniqueId and SourceUniqueId based on composite actions and revert. Currently UniqueId is never used as
 // it's available always the same way SourceUniqueId is and SourceUniqueId is universal identifier for response actions.
 // This makes SDID more suitable in current code base as main Id during creating entities, while UniqueId has no role
 // other than synchronization of Client and Server in batch.


// Original tile system
// TYPEENUM Each tile is either part of world or border around the world. This can be used to determine additional behaviour.
// REGION TYPE Representation of tile most important part, be it Underground, Ground or Sky...
// BIOME TYPE Represents primary weather, fauna, flora condition of a tile such as Desert, Temperate, Tropical, Alpine, Tundra...
// FEATURES TYPE[] Represents various features such as River, Lake, Ocean, Lava Lake, Volcano, Mountain, Hill, Canyon, Ravine, Forest, Jungle, Plain...
// EXTENSION TYPE[] Represents various special features add to Tile such as Settlement, Capital City, Improvements...

