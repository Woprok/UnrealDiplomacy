// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModifierManager.generated.h"

// Forward Declarations

struct FUDModifierData;
struct FUDModifierPresentation;
class IUDModifierInterface;
class UUDTileState;
class UUDFactionState;

/**
 * Manages all modifiers that are allowed to exist.
 * Each modifiers must be registered with this manager otherwise
 * it will not be assignable.
 *
 * Provides basic filters to retrieve information about modifiers.
 * This is stateless and only keeps references to his childs, that are as well stateless.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDModifierManager : public UObject
{
	GENERATED_BODY()
#pragma region Modifier Creation API
public:
	/**
	 * Creates tile modifier to a specified tile.
	 */
	void CreateTileModifier(const TObjectPtr<UUDTileState>& tile, FUDModifierData newModifier);
	/**
	 * Creates faction modifier to a specified faction.
	 */
	void CreateFactionModifier(const TObjectPtr<UUDFactionState>& faction, FUDModifierData newModifier);
	/**
	 * Removes tile modifier from a specified tile based on Action that created it.
	 */
	void RemoveTileModifier(const TObjectPtr<UUDTileState>& tile, int32 actionUniqueId);
	/**
	 * Removes faction modifier from a specified faction based on Action that created it.
	 */
	void RemoveFactionModifier(const TObjectPtr<UUDFactionState>& faction, int32 actionUniqueId);
	/**
	 * Checks if specified tile modifier on a specified tile is present.
	 * This requires modifier that would be applied amd will use value compare.
	 * See FUDModifierData::IsTypeTargetEqual
	 */
	bool HasTileModifier(const TObjectPtr<UUDTileState>& tile, FUDModifierData searchedModifier) const;
	/**
	 * Checks if specified faction modifier on a specified faction is present.
	 * This requires modifier that would be applied amd will use value compare.
	 * See FUDModifierData::IsTypeTargetEqual
	 */
	bool HasFactionModifier(const TObjectPtr<UUDFactionState>& faction, FUDModifierData searchedModifier) const;
	/**
	 * Get all modifiers with the specified Id that are currently present on the specified faction.
	 */
	TArray<FUDModifierData> GetAllFactionModifiers(const TObjectPtr<UUDFactionState>& faction, int32 modifierId);
#pragma endregion
public:
	/**
	 * Returns Modifier associated with the supplied Id.
	 */
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IUDModifierInterface> GetModifier(int32 modifierTypeId);
	/**
	 * Returns true if modifier was registered succeffully.
	 * This can fail if modifier has duplicate id with already existing one.
	 */
	UFUNCTION(BlueprintCallable)
	bool RegisterModifier(TScriptInterface<IUDModifierInterface> newModifier);
	/**
	 * Safely initializes all dependencies.
	 */
	UFUNCTION(BlueprintCallable)
	void Initialize();
#pragma region Modifier Type Filter API
public:
	/**
	 * Retrieves all modifiers that are tagged as faction modifiers.
	 * Faction modifier is any modifier that is saved in faction.
	 */
	TArray<FUDModifierPresentation> FilterFactionModifiers();
	/**
	 * Retrieves all modifiers that are tagged as tile modifiers.
	 * Tile modifier is any modifiers that is saved in tile directly.
	 */
	TArray<FUDModifierPresentation> FilterTileModifiers();
	/**
	 * Retrieves specifeid modifier presentation.
	 */
	FUDModifierPresentation GetSpecified(int32 modifierId);
private:
	/**
	 * Retrieves all modifiers that are tagged by specified tag.
	 */
	TArray<FUDModifierPresentation> FilterByTag(const TArray<FUDModifierPresentation>& selection, int32 tag);
	/**
	 * Tags are not changing during the game and this serves as filter startpoint.
	 */
	UPROPERTY()
	TArray<FUDModifierPresentation> FilterStartpoint = { };
#pragma endregion
protected:
	/** Registers all core modifiers. */
	UFUNCTION()
	void RegisterCoreModifiers();
	/** Registers all blueprint modifiers. This takes precedence over the core. */
	void RegisterBlueprintModifiers();
	/** Default method for registering multiple modifiers at once. */
	UFUNCTION()
	void RegisterModifierList(TArray<TScriptInterface<IUDModifierInterface>> modifierList);
	/** All modifiers registered with the manager. */
	UPROPERTY()
	TMap<int32, TScriptInterface<IUDModifierInterface>> Modifiers;
	/** Enables blueprint defined and overrides for registration. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifiers")
	TArray<TSubclassOf<UObject>> BlueprintModifiers;
private:
	/** Determines if this was initialized and can be used. */
	UPROPERTY()
	bool IsInitialized = false;
};