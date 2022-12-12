// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModifier.generated.h"

/**
 * Modifier is generic interface that can be assigned to an object.
 * Action's are then able to use that to determine if they can use it and
 * proceed to cast it to proper class to obtain all necessary informations.
 * This is required by UE compiler. This can't contain UNREALDIPLOMACY_API macro.
 */
//UINTERFACE(MinimalAPI, Blueprintable)
//class UUDModifier : public UInterface
//{
//	GENERATED_BODY()
//};

/**
 * Modifier is generic interface that can be assigned to an object.
 * Action's are then able to use that to determine if they can use it and
 * proceed to cast it to proper class to obtain all necessary informations.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDModifier : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Unique modifier id that is provided for each modifier.
	 * This is directly referenced by actions and ModifierManager to determine,
	 * how to use this and what kind of data it might contain.
	 * This has to be overriden, otherwise the modifier might be discarded as invalid (valid values are 0 - int32.MAX).
	 */
	virtual int32 GetModifierTypeId()
	{
		// Default Interface call returns invalid value, e.g. -1.
		return -1;
	}
	/**
	 * Unique id that is assigned to each action that is executed.
	 * Used for managing collections.
	 */
	virtual int32 GetAssociatedActionUniqueId()
	{
		return UniqueActionId;
	}
protected:
	int32 UniqueActionId;
};

/**
 * Permission modifier that is applied to a Tile.
 * Enables Tile to be used by non-owner.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDExploitTilePermissionModifier : public UUDModifier
{
	GENERATED_BODY()
public:
	static TObjectPtr<UUDModifier> Create(int32 uniqueActionId, int32 playerId)
	{
		TObjectPtr<UUDExploitTilePermissionModifier> modifier = NewObject<UUDExploitTilePermissionModifier>();
		//modifier->AddToRoot();
		modifier->UniqueActionId = uniqueActionId;
		modifier->PlayerId = playerId;
		return modifier;
	};
	
	int32 PlayerId = -1;

	virtual int32 GetModifierTypeId() override { return ModifierTypeId; };
	static const int32 ModifierTypeId = 1;
};