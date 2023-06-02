// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDControllerInterface.generated.h"

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
};