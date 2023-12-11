// Copyright Miroslav Valach

#include "Core/Simulation/UDActionManager.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDWorldFactionGenerator.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDStratagemOperationManager.h"
#include "Core/UDGlobalData.h"
#include "Core/UDGameInstance.h"
// Actions
#include "Core/Simulation/Actions/UDActionDatabase.h"

void UUDActionManager::SetModifierManager(TWeakObjectPtr<UUDModifierManager> manager)
{
	ModifierManager = manager;
}

void UUDActionManager::SetResourceManager(TWeakObjectPtr<UUDResourceManager> manager)
{
	ResourceManager = manager;
}

void UUDActionManager::SetStratagemOperationManager(TWeakObjectPtr<UUDStratagemOperationManager> manager)
{
	StratagemOperationManager = manager;
}

TWeakObjectPtr<UUDModifierManager> UUDActionManager::GetModifierManager()
{
	return ModifierManager;
}

TWeakObjectPtr<UUDResourceManager> UUDActionManager::GetResourceManager()
{
	return ResourceManager;
}

TWeakObjectPtr<UUDStratagemOperationManager> UUDActionManager::GetStratagemOperationManager()
{
	return StratagemOperationManager;
}

TScriptInterface<IUDActionInterface> UUDActionManager::GetAction(int32 actionTypeId)
{
	check(IsInitialized);
	if (!Actions.Contains(actionTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Action(%d) is not defined!"), actionTypeId);
		return Actions[UUDGlobalData::InvalidActionId];
	}

	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Retrieved Action(%d)."), actionTypeId);
	return Actions[actionTypeId];
}

bool UUDActionManager::RegisterAction(TScriptInterface<IUDActionInterface> newAction)
{
	check(IsInitialized);
	if (Actions.Contains(newAction->GetId()))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Duplicate registration of Action(%d)."), newAction->GetId());
		return false;
	}

	BindSharedToAction(newAction);

	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Action(%d) successfully registered."), newAction->GetId());
	Actions.Add(newAction->GetId(), newAction);
	FilterStartpoint.Add(newAction->GetPresentation());
	return true;
}

void UUDActionManager::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Initializing new instance of ActionManager."));
	check(!IsInitialized);
	IsInitialized = true;

	WorldGenerator = NewObject<UUDWorldGenerator>(this);
	WorldGenerator->SetResourceManager(ResourceManager);

	WorldFactionGenerator = NewObject<UUDWorldFactionGenerator>(this);
	WorldFactionGenerator->SetResourceManager(ResourceManager);

	RegisterBlueprintActions();
	RegisterCoreActions();
}

void UUDActionManager::RegisterCoreActions()
{
	RegisterActionList(UUDActionDatabase::GetDefaultActions(this));
	RegisterActionList(UUDActionDatabase::GetDecisionActions(this));
	RegisterActionList(UUDActionDatabase::GetGaiaActions(this));
	RegisterActionList(UUDActionDatabase::GetSystemActions(this));
	RegisterActionList(UUDActionDatabase::GetGameActions(this));
	RegisterActionList(UUDActionDatabase::GetDealActions(this));
	RegisterActionList(UUDActionDatabase::GetSettingActions(this));
}

void UUDActionManager::RegisterBlueprintActions()
{
	TArray<TScriptInterface<IUDActionInterface>> newActions = { };

	for (const auto& actionType : BlueprintActions)
	{
		// Check if this is actually a valid item.
		if (!actionType)
		{
			UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Failed to initialize action type. Probably empty array item."));
			continue;
		}
		// Create an instance of the class
		UObject* newActionObject = NewObject<UObject>(this, actionType);

		if (!newActionObject)
		{
			UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Failed to initialize action type instance."));
			continue;
		}

		// Check if the object implements the desired interface
		if (!newActionObject->GetClass()->ImplementsInterface(UUDActionInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("UUDResourceManager: Incorrect action type."));
			continue;
		}
		// Cast to the interface
		TScriptInterface<IUDActionInterface> ResourceInterface;
		ResourceInterface.SetObject(newActionObject);
		ResourceInterface.SetInterface(Cast<IUDActionInterface>(newActionObject));
		newActions.Add(ResourceInterface);
	}

	RegisterActionList(newActions);
}

void UUDActionManager::RegisterActionList(TArray<TScriptInterface<IUDActionInterface>> actionList)
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionManager: Attempting to register action list."));

	int32 failCount = 0;
	for (auto& action : actionList)
	{
		if (!RegisterAction(action))
		{
			++failCount;
		}
	}

	UE_LOG(LogTemp, Log, 
		TEXT("UUDActionManager: Registering action list finished with %d failures out of %d actions."),
		failCount,
		actionList.Num());
}

void UUDActionManager::BindSharedToAction(TScriptInterface<IUDActionInterface> newAction)
{
	newAction->SetWorldGenerator(WorldGenerator);
	newAction->SetWorldFactionGenerator(WorldFactionGenerator);
	newAction->SetModifierManager(ModifierManager);
	newAction->SetResourceManager(ResourceManager);
}

#pragma region Action Filter API

TArray<FUDActionPresentation> UUDActionManager::FilterStratagems()
{
	return FilterByTag(FilterStartpoint, UD_ACTION_TAG_STRATAGEM);
}

TArray<FUDActionPresentation> UUDActionManager::FilterFactionInteractions(int32 additionalTag)
{
	TSet<int32> filterTags = { UD_ACTION_TAG_FACTION_INTERACTION, additionalTag };
	return FilterByTags(FilterStartpoint, filterTags);
}

TArray<FUDActionPresentation> UUDActionManager::FilterTileInteractions()
{
	return FilterByTag(FilterStartpoint, UD_ACTION_TAG_TILE_INTERACTION);
}

FUDActionPresentation UUDActionManager::GetSpecified(int32 actionId)
{
	return Actions[actionId]->GetPresentation();
}

TArray<FUDActionPresentation> UUDActionManager::FilterByTag(const TArray<FUDActionPresentation>& selection, int32 tag)
{
	TArray<FUDActionPresentation> filtered = { };
	for (const auto& action : selection)
	{
		if (action.Tags.Contains(tag))
		{
			filtered.Add(action);
		}
	}

	return filtered;
}

TArray<FUDActionPresentation> UUDActionManager::FilterByTags(const TArray<FUDActionPresentation>& selection, TSet<int32> tags)
{
	TArray<FUDActionPresentation> filtered = { };
	for (const auto& action : selection)
	{
		if (action.Tags.Includes(tags))
		{
			filtered.Add(action);
		}
	}

	return filtered;
}

#pragma endregion