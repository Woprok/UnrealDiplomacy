// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDControllerInterface.generated.h"

/**
 * Interface for setting and retrieving unique Id assigned by GameMode to players.
 * This is required by UE compiler. This can't contain UNREALDIPLOMACY_API macro.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UUDControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for setting and retrieving unique Id assigned by GameMode to players.
 */
class UNREALDIPLOMACY_API IUDControllerInterface
{
	GENERATED_BODY()
public:
	/**
	 * Set unique Id used by this controller to identify yourself.
	 */
	virtual void SetControllerUniqueId(int32 uniqueControllerId) 
	{
		// Default Interface call is empty.
	}
	/**
	 * Returns unique Id assigned to this controller.
	 * This has to be overriden, otherwise the controller might be discarded as invalid (valid values are 0 - int32.MAX).
	 */
	virtual int32 GetControllerUniqueId()
	{
		// Default Interface call returns invalid value, e.g. -1.
		return -1;
	}
};