// Copyright Miroslav Valach

#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDResourceData.h"
#include "Core/Simulation/UDResourceInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Resources/UDResourceDatabase.h"
#include "Core/UDGlobalData.h"

#pragma region Resource Creation API
void UUDResourceManager::SetupFactionStartingResources(TObjectPtr<UUDFactionState> faction)
{
	for (auto& resource : FilterStartpoint)
	{
		if (resource.Tags.Contains(UD_RESOURCE_TAG_FACTION_STARTING))
			faction->Resources.Add(resource.ResourceId, resource.FactionStartingAmount);
	}
}

TArray<FUDResourcePresentation> UUDResourceManager::CreateTileResourceOptionArray()
{
	TArray<FUDResourcePresentation> resources = { };
	for (auto& resource : FilterStartpoint)
	{
		if (resource.Tags.Contains(UD_RESOURCE_TAG_TILE_STARTING) && resource.TileWeight > 0)
		{
			// Only resources with non-zero chance are relevant for generation.
			resources.Add(resource);
		}
	}
	return resources;
}

int32 UUDResourceManager::GetCurrent(TObjectPtr<UUDFactionState> faction, int32 resourceId)
{
	return faction->Resources[resourceId];
}

#pragma endregion

#pragma region Resource Use API
void UUDResourceManager::Gain(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount)
{
	faction->Resources[resourceId] += amount;
}

void UUDResourceManager::Lose(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount)
{
	faction->Resources[resourceId] -= amount;
}

bool UUDResourceManager::CanSpend(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount)
{
	return faction->Resources[resourceId] >= amount;
}

void UUDResourceManager::Add(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount)
{
	faction->Resources[resourceId] += amount;
}

void UUDResourceManager::Substract(TObjectPtr<UUDFactionState> faction, int32 resourceId, int32 amount)
{
	faction->Resources[resourceId] -= amount;
}
#pragma endregion

TScriptInterface<IUDResourceInterface> UUDResourceManager::GetResource(int32 resourceTypeId)
{
	check(IsInitialized);
	if (!Resources.Contains(resourceTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Resource(%d) is not defined!"), resourceTypeId);
		return Resources[UUDGlobalData::InvalidResourceId];
	}

	UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Retrieved Resource(%d)."), resourceTypeId);
	return Resources[resourceTypeId];
}

bool UUDResourceManager::RegisterResource(TScriptInterface<IUDResourceInterface> newResource)
{
	check(IsInitialized);
	if (Resources.Contains(newResource->GetId()))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Duplicate registration of Resource(%d)."), newResource->GetId());
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Resource(%d) successfully registered."), newResource->GetId());
	Resources.Add(newResource->GetId(), newResource);
	FilterStartpoint.Add(newResource->GetPresentation());
	return true;
}

void UUDResourceManager::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Initializing new instance of ResourceManager."));
	check(!IsInitialized);
	IsInitialized = true;

	RegisterBlueprintResources();
	RegisterCoreResources();
}

void UUDResourceManager::RegisterCoreResources()
{
	RegisterResourceList(UUDResourceDatabase::GetDefaultResources(this));
	RegisterResourceList(UUDResourceDatabase::GetGameResources(this));
}

void UUDResourceManager::RegisterBlueprintResources()
{
	TArray<TScriptInterface<IUDResourceInterface>> newResources = { };

	for (const auto& resourceType : BlueprintResources)
	{
		// Check if this is actually a valid item.
		if (!resourceType)
		{
			UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Failed to initialize resource type. Probably empty array item."));
			continue;
		}
		// Create an instance of the class
		UObject* newResourceObject = NewObject<UObject>(this, resourceType);

		if (!newResourceObject)
		{
			UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Failed to initialize resource type instance."));
			continue;
		}

		// Check if the object implements the desired interface
		if (!newResourceObject->GetClass()->ImplementsInterface(UUDResourceInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Incorrect resource type."));
			continue;
		}
		// Cast to the interface
		TScriptInterface<IUDResourceInterface> ResourceInterface;
		ResourceInterface.SetObject(newResourceObject);
		ResourceInterface.SetInterface(Cast<IUDResourceInterface>(newResourceObject));
		newResources.Add(ResourceInterface);
	}

	RegisterResourceList(newResources);
}

void UUDResourceManager::RegisterResourceList(TArray<TScriptInterface<IUDResourceInterface>> resourceList)
{
	UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Attempting to register resource list."));

	int32 failCount = 0;
	for (auto& resource : resourceList)
	{
		if (!RegisterResource(resource))
		{
			++failCount;
		}
	}

	UE_LOG(LogTemp, Log,
		TEXT("UUDResourceManager: Registering resource list finished with %d failures out of %d resources."),
		failCount,
		resourceList.Num());
}

#pragma region Resource Filter API

TArray<FUDResourcePresentation> UUDResourceManager::FilterResources()
{
	return FilterByTag(FilterStartpoint, UD_RESOURCE_TAG_VALID);
}

TArray<FUDResourcePresentation> UUDResourceManager::FilterPrimaryResources()
{
	return FilterByTag(FilterStartpoint, UD_RESOURCE_TAG_TYPE_PRIMARY);
}

TArray<FUDResourcePresentation> UUDResourceManager::FilterSecondaryResources()
{
	return FilterByTag(FilterStartpoint, UD_RESOURCE_TAG_TYPE_SECONDARY);
}

FUDResourcePresentation UUDResourceManager::GetSpecified(int32 resourceId)
{
	return Resources[resourceId]->GetPresentation();
}

UMaterialInterface* UUDResourceManager::GetIcon(int32 resourceId)
{
	return Resources[resourceId]->GetIcon();
}

TArray<FUDResourcePresentation> UUDResourceManager::FilterByTag(const TArray<FUDResourcePresentation>& selection, int32 tag)
{
	TArray<FUDResourcePresentation> filtered = { };
	for (const auto& action : selection)
	{
		if (action.Tags.Contains(tag))
		{
			filtered.Add(action);
		}
	}

	return filtered;
}

#pragma endregion