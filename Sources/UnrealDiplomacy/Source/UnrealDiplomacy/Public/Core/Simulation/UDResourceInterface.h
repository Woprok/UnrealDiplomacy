// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDResourceInterface.generated.h"

// Forward Declarations

struct FUDResourcePresentation;

// TODO Reevaluate if this tag system can be done it in different way to reduce complexity.

// Useable for internal purposes only.
#define UD_RESOURCE_TAG_INVALID 0
// Useable for external purposes as well.
#define UD_RESOURCE_TAG_VALID 1
// Specific details about resources.
#define UD_RESOURCE_TAG_TYPE_PRIMARY 2
#define UD_RESOURCE_TAG_TYPE_SECONDARY 3
// Resources used by the game.
// TODO move this to resource manager. IN PROGRESS
//#define UD_RESOURCE_REPUTATION_ID 0
//#define UD_RESOURCE_GOLD_ID 1
//#define UD_RESOURCE_FOOD_ID 2
//#define UD_RESOURCE_MATERIALS_ID 3
//#define UD_RESOURCE_LUXURIES_ID 4
//#define UD_RESOURCE_MANPOWER_ID 5

/**
 * Defines informations about the modifier and how it can be displayed in UI.
 */
USTRUCT()
struct FUDResourcePresentation
{
	GENERATED_BODY()
public:
	FUDResourcePresentation() {}
	/** Unique Resource Id. */
	int32 ResourceId;
	/** List of all tags that are used to define behaviour in UI, etc... */
	TSet<int32> Tags;
	/** Main name of this modifier. */
	FString Name = TEXT("");
	/**
	 * Defines format for converting modifier to offer modifier string based on modifier parameters.
	 * To use this use format and supply all parameters required by tags.
	 */
	FString ResourceDescriptionFormat = TEXT("");
};

/**
 * Stumb for Resource Interface.
 * Unreal Engine requires Interface to be first defined as empty stumb.
 * https://docs.unrealengine.com/5.2/en-US/interfaces-in-unreal-engine/
 */
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UUDResourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Resources are defined as separate entities to simplify their integration in other parts.
 */
class UNREALDIPLOMACY_API IUDResourceInterface
{
	GENERATED_BODY()
public:
	/**
	 * Modifiers are identified by their unique Id.
	 * Valid values are 0 - int32.MAX.
	 * Only UUDSystemActionInvalid and interface are allowed to return -1;
	 */
	virtual int32 GetId() const;
	/**
	 * Formatted version of the modifier.
	 */
	virtual FString ToString() const;
	/**
	 * Contains definition for filtering and displaying modifier.
	 */
	virtual FUDResourcePresentation GetPresentation() const;
};