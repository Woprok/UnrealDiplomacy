// Copyright Miroslav Valach

#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDResourceData.h"
#include "Core/Simulation/UDResourceInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Resources/UDResourceDatabase.h"
#include "Core/UDGlobalData.h"

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

	RegisterCoreResources();
	RegisterAdditionalResources();
}

void UUDResourceManager::RegisterCoreResources()
{
	RegisterResourceList(UUDResourceDatabase::GetDefaultResources(this));
	RegisterResourceList(UUDResourceDatabase::GetGameResources(this));
}

void UUDResourceManager::RegisterAdditionalResources()
{
	// This is only for inherited managers.
	return;
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

#pragma region Resource Creation API

#pragma endregion