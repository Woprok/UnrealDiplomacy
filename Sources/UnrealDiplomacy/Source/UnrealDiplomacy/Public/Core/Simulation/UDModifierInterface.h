// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModifierInterface.generated.h"

// Forward Declarations

struct FUDModifierPresentation;

// TODO Reevaluate if this tag system can be done it in different way to reduce complexity.

// Useable for internal purposes only.
#define UD_MODIFIER_TAG_INVALID 0
// Useable for external purposes as well.
#define UD_MODIFIER_TAG_VALID 1
// Applied as faction modifier to faction.
#define UD_MODIFIER_TAG_TYPE_FACTION 2
// Applied as tile modifier to tile.
#define UD_MODIFIER_TAG_TYPE_TILE 3
// Specific parameter that should be included in parameter list.
#define UD_MODIFIER_TAG_PARAMETER_ACTION 15

/**
 * Defines informations about the modifier and how it can be displayed in UI.
 */
USTRUCT()
struct FUDModifierPresentation
{
	GENERATED_BODY()
public:
	FUDModifierPresentation() {}
	/** Unique Modifier Id. */
	int32 ModifierId;
	/** List of all tags that are used to define behaviour in UI, etc... */
	TSet<int32> Tags;
	/** Main name of this modifier. */
	FString Name = TEXT("");
	/**
	 * Defines format for converting modifier to offer modifier string based on modifier parameters.
	 * To use this use format and supply all parameters required by tags.
	 */
	FString ModifierDescriptionFormat = TEXT("");
};

/**
 * Stumb for Modifier Interface.
 * Unreal Engine requires Interface to be first defined as empty stumb.
 * https://docs.unrealengine.com/5.2/en-US/interfaces-in-unreal-engine/
 */
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UUDModifierInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Modifiers are special data carries that are used by actions to communicate certain facts.
 * Such as alliancies, sharing of tiles or duration of the deals.
 */
class UNREALDIPLOMACY_API IUDModifierInterface
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
	 * Additional parameters required by the modifier.
	 * Valid values are 0 - int32.MAX.
	 */
	virtual int32 GetParameterCount() const;
	/**
	 * Formatted version of the modifier.
	 */
	virtual FString ToString() const;
	/**
	 * Contains definition for filtering and displaying modifier.
	 */
	virtual FUDModifierPresentation GetPresentation() const;
};