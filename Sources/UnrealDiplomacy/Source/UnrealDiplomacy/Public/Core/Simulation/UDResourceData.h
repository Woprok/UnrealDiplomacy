// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDResourceData.generated.h"

/**
 * Simple definition for resource data.
 * TODO implement use of this.
 *
 * Refer to actions about behaviour details that are not mentioned here.
 */
USTRUCT(BlueprintType)
struct FUDResourceData
{
	GENERATED_BODY()
public:
	FUDResourceData();
	/**
	 * Copy constructor used by UE.
	 * Please do not copy resources, it will cause errors!
	 */
	FUDResourceData(const FUDResourceData& existingResource);
	/** Returns debug friendly string from this action data. */
	FString ToString() const;
public:
	/**
	 * Equality over ActionUniqueId field.
	 */
	inline bool operator!=(const FUDResourceData& rhs) const
	{
		return !(*this == rhs);
	}
	/**
	 * Equality over ActionUniqueId field.
	 */
	inline bool operator==(const FUDResourceData& rhs) const
	{
		return ResourceId == rhs.ResourceId;
	}
	/**
	 * Equality over all value fields without ActionUniqueId.
	 * This means that both share same data, but might be created by different actions.
	 * Sharing data is still too strong condition for searching modifiers...
	 * This is primarily whenever you need to ensure that duplicates do not exists.
	 */
	inline bool IsValueEqual(const FUDResourceData& rhs) const
	{
		return
			ResourceId == rhs.ResourceId;
	}
	/**
	 * Equality over type and target only.
	 * For use whenever we are interested only in knowing about modifier existance.
	 */
	inline bool IsTypeTargetEqual(const FUDResourceData& rhs) const
	{
		return
			ResourceId == rhs.ResourceId;
	}
public:
	/**
	 * Id used by resource interface.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 ResourceId = -1;
};