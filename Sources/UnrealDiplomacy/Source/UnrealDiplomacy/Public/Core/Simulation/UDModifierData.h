// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModifierData.generated.h"

/**
 * Simple for of modifiers with as much space for future expansion as possible.
 * Modifiers are stored as data inside of WorldState.
 * Either in TileState or FactionState.
 * This means that there are two types of states that can appear.
 * Tile modifier has implicit tile, based on tile it's part of.
 * Faction modifier has implicit faction, based on faction it's part of, but that is same as TargetId;
 * TargetId is always required as well as the InvokerId so modifier can be properly associated.
 * 
 * Refer to actions about behaviour details that are not mentioned here.
 */
USTRUCT(BlueprintType)
struct FUDModifierData 
{
	GENERATED_BODY()
public:
	FUDModifierData();
	/**
	 * Copy constructor used by UE.
	 * Please do not copy modifiers, it will cause errors!
	 */
	FUDModifierData(const FUDModifierData& existingModifier);
	/**
	 * Standard constructor for most modifiers.
	 */
	FUDModifierData(int32 modifierTypeId, int32 actionUniqueId, int32 creatorId, int32 targetId);
	/**
	 * Standard constructor for most modifiers that have additional parameters.
	 */
	FUDModifierData(int32 modifierTypeId, int32 actionUniqueId, int32 creatorId, int32 targetId, TArray<int32> valuePrameters);
	/**
	 * Returns debug friendly string from this action data.
	 */
	FString ToString() const;
public:
	/**
	 * Equality over ActionUniqueId field.
	 */
	inline bool operator!=(const FUDModifierData& rhs) const
	{
		return !(*this == rhs);
	}
	/**
	 * Equality over ActionUniqueId field.
	 */
	inline bool operator==(const FUDModifierData& rhs) const
	{
		return ActionUniqueId == rhs.ActionUniqueId;
	}
	/**
	 * Equality over all value fields without ActionUniqueId.
	 * This means that both share same data, but might be created by different actions.
	 * Sharing data is still too strong condition for searching modifiers...
	 * This is primarily whenever you need to ensure that duplicates do not exists.
	 */
	inline bool IsValueEqual(const FUDModifierData& rhs) const
	{
		return
			ModifierTypeId == rhs.ModifierTypeId &&
			InvokerId == rhs.InvokerId &&
			TargetId == rhs.TargetId &&
			ValueParameters == rhs.ValueParameters;
	}	
	/**
	 * Equality over type and target only.
	 * For use whenever we are interested only in knowing about modifier existance.
	 */
	inline bool IsTypeTargetEqual(const FUDModifierData& rhs) const
	{
		return
			ModifierTypeId == rhs.ModifierTypeId &&
			TargetId == rhs.TargetId;
	}
public:
	/**
	 * Id used by modifier interface.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 ModifierTypeId = -1;
	/**
	 * Each modifier is exclusively associated with single action.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 ActionUniqueId = -1;
	/**
	 * Faction responsible for creating this modifier.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 InvokerId = -1;
	/**
	 * Faction targeted by this modifier.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = -1;
	/**
	 * Optional parameters...
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ValueParameters;
};