// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDResourceManager.generated.h"

// Forward Declarations

struct FUDResourceData;
struct FUDResourcePresentation;
class IUDResourceInterface;
class UUDFactionState;
class UUDTileState;

/**
 * Manages all resources that are allowed to exist.
 * Each resource must be registered with this manager otherwise
 * it will not be assignable or useable.
 *
 * Provides basic filters to retrieve information about resources.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDResourceManager : public UObject
{
	GENERATED_BODY()

#pragma region Resource Creation API
public:
	/** Updates faction state to contain all starting resources and their correct amount. */
	void SetupFactionStartingResources(TObjectPtr<UUDFactionState> faction);
	/** Updates tile state to contain starting resources and correct starting amount. */
	void GenerateTileStartingResources(TObjectPtr<UUDTileState> tile);
#pragma endregion
#pragma region Resource Use API
public:
	/** Returns current amount of the specified resource. */
	int32 GetCurrent(TObjectPtr<UUDFactionState> faction, int32 resourceId);
	/** Applies all modifiers and increses amount of resource for specified faction. Negates Lose. */
	void Gain(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount);
	/** Applies all modifiers and decresases amount of resource for specified faction. Negates Gain. */
	void Lose(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount);
	/** Checks if specified faction can spend amount of resource. */
	bool CanSpend(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount);
	/** Increses amount of resource for specified faction. Negates Substract */
	void Add(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount);
	/** Decreases amount of resource for specified faction. Negates Add */
	void Substract(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount);
#pragma endregion
public:
	/** Returns Resource associated with the supplied Id. */
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IUDResourceInterface> GetResource(int32 resourceId);
	/**
	 * Returns true if resource was registered succeffully.
	 * This can fail if resource has duplicate id with already existing one.
	 */
	UFUNCTION(BlueprintCallable)
	bool RegisterResource(TScriptInterface<IUDResourceInterface> newResource);
	/** Safely initializes all dependencies. */
	UFUNCTION(BlueprintCallable)
	void Initialize();
#pragma region Resource Type Filter API
public:
	/** Retrieves all resources that are tagged as valid, ignores tag such as primary... */
	TArray<FUDResourcePresentation> FilterResources();
	/** Retrieves all resources that are tagged as primary. */
	TArray<FUDResourcePresentation> FilterPrimaryResources();
	/** Retrieves all resources that are tagged as secondary. */
	TArray<FUDResourcePresentation> FilterSecondaryResources();
	/** Retrieves specifeid resource presentation. */
	FUDResourcePresentation GetSpecified(int32 resourceId);
private:
	/** Retrieves all resources that are tagged by specified tag. */
	TArray<FUDResourcePresentation> FilterByTag(const TArray<FUDResourcePresentation>& selection, int32 tag);
	/** Tags are not changing during the game and this serves as filter startpoint.  */
	UPROPERTY()
	TArray<FUDResourcePresentation> FilterStartpoint = { };
#pragma endregion
protected:
	/** Registers all core resources. */
	UFUNCTION()
	void RegisterCoreResources();
	/** Registers all additional resources. */
	UFUNCTION(BlueprintCallable)
	virtual void RegisterAdditionalResources();
	/** Default method for registering multiple resources at once.  */
	UFUNCTION()
	void RegisterResourceList(TArray<TScriptInterface<IUDResourceInterface>> resourceList);
	/** All resources registered with the manager. */
	UPROPERTY()
	TMap<int32, TScriptInterface<IUDResourceInterface>> Resources;
private:
	/** Determines if this was initialized and can be used. */
	UPROPERTY()
	bool IsInitialized = false;
};