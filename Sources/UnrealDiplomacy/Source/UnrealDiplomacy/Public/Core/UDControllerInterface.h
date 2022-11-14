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
	virtual void SetControllerUniqueId(int32 uniqueControllerId);
	virtual int32 GetControllerUniqueId();
};