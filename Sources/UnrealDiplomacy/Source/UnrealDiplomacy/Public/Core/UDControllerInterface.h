// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDControllerInterface.generated.h"

/**
 * Defines all available types of controllers.
 */
UENUM(BlueprintType)
enum class EUDControllerType : uint8
{
	/**
	 * Undefined value, this controller should be discarded and error should be thrown.
	 */
	Invalid = 0,
	/**
	 * Menu or any other non match state.
	 */
	Menu = 1,
	/**
	 * Server controller during the match.
	 */
	Server = 2,
	/**
	 * AI controller during the match.
	 */
	AI = 3,
	/**
	 * Human player during the match.
	 */
	Player = 4,
	/**
	 * Human spectator during the match.
	 */
	Observer = 5,
};

/**
 * Stumb for Action Interface.
 * Unreal Engine requires Interface to be first defined as empty stumb.
 * https://docs.unrealengine.com/5.2/en-US/interfaces-in-unreal-engine/
 */
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UUDControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Gamemode associated each controller with unique id.
 * Each controller can control single faction.
 * Multiple controllers can control same faction.
 */
class UNREALDIPLOMACY_API IUDControllerInterface
{
	GENERATED_BODY()
public:
	/**
	 * Change unique id associated with this controller.
	 */
	virtual void SetControllerUniqueId(int32 uniqueControllerId);
	/**
	 * Returns unique Id assigned to this controller.
	 * Always return last set unique id.
	 * Valid values are 0 - int32.MAX. 
	 */
	virtual int32 GetControllerUniqueId();
	/**
	 * Change controlled faction id.
	 * Clients should follow this by changing their perspective.
	 */
	virtual void SetControlledFactionId(int32 factionId);
	/**
	 * Retrieve controlled faction id.
	 */
	virtual int32 GetControlledFactionId();
	/**
	 * Changes the type to specified.
	 * This is primarily used by server to discern controller types.
	 */
	virtual void SetControllerType(EUDControllerType type);
	/**
	 * Retrives current type as set by default on creation or changed by game mode.
	 * Default is Menu, which represents any non playable state.
	 */
	virtual EUDControllerType GetControllerType();
};