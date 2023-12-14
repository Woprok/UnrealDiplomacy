// Copyright Miroslav Valach

#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Modifiers/UDModifierDatabase.h"
#include "Core/UDGlobalData.h"

TScriptInterface<IUDModifierInterface> UUDModifierManager::GetModifier(int32 modifierTypeId)
{
	check(IsInitialized);
	if (!Modifiers.Contains(modifierTypeId))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Modifier(%d) is not defined!"), modifierTypeId);
		return Modifiers[UUDGlobalData::InvalidModifierId];
	}

	UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Retrieved Modifier(%d)."), modifierTypeId);
	return Modifiers[modifierTypeId];
}

bool UUDModifierManager::RegisterModifier(TScriptInterface<IUDModifierInterface> newModifier)
{
	check(IsInitialized);
	if (Modifiers.Contains(newModifier->GetId()))
	{
		UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Duplicate registration of Modifier(%d)."), newModifier->GetId());
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Modifier(%d) successfully registered."), newModifier->GetId());
	Modifiers.Add(newModifier->GetId(), newModifier);
	FilterStartpoint.Add(newModifier->GetPresentation());
	return true;
}

void UUDModifierManager::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Initializing new instance of ModifierManager."));
	check(!IsInitialized);
	IsInitialized = true;

	RegisterBlueprintModifiers();
	RegisterCoreModifiers();
}

void UUDModifierManager::RegisterCoreModifiers()
{
	RegisterModifierList(UUDModifierDatabase::GetDefaultModifiers(this));
	RegisterModifierList(UUDModifierDatabase::GetTileModifiers(this));
	RegisterModifierList(UUDModifierDatabase::GetFactionModifiers(this));
}

void UUDModifierManager::RegisterBlueprintModifiers()
{
	TArray<TScriptInterface<IUDModifierInterface>> newModifiers = { };

	for (const auto& modifierType : BlueprintModifiers)
	{
		// Check if this is actually a valid item.
		if (!modifierType)
		{
			UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Failed to initialize modifier type. Probably empty array item."));
			continue;
		}
		// Create an instance of the class
		UObject* newModifierObject = NewObject<UObject>(this, modifierType);

		if (!newModifierObject)
		{
			UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Failed to initialize modifier type instance."));
			continue;
		}

		// Check if the object implements the desired interface
		if (!newModifierObject->GetClass()->ImplementsInterface(UUDModifierInterface::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Incorrect modifier type."));
			continue;
		}
		// Cast to the interface
		TScriptInterface<IUDModifierInterface> ResourceInterface;
		ResourceInterface.SetObject(newModifierObject);
		ResourceInterface.SetInterface(Cast<IUDModifierInterface>(newModifierObject));
		newModifiers.Add(ResourceInterface);
	}

	RegisterModifierList(newModifiers);
}

void UUDModifierManager::RegisterModifierList(TArray<TScriptInterface<IUDModifierInterface>> modifierList)
{
	UE_LOG(LogTemp, Log, TEXT("UUDModifierManager: Attempting to register modifier list."));

	int32 failCount = 0;
	for (auto& modifier : modifierList)
	{
		if (!RegisterModifier(modifier))
		{
			++failCount;
		}
	}

	UE_LOG(LogTemp, Log,
		TEXT("UUDModifierManager: Registering modifier list finished with %d failures out of %d modifiers."),
		failCount,
		modifierList.Num());
}

#pragma region Modifier Filter API

TArray<FUDModifierPresentation> UUDModifierManager::FilterFactionModifiers()
{
	return FilterByTag(FilterStartpoint, UD_MODIFIER_TAG_TYPE_FACTION);
}

TArray<FUDModifierPresentation> UUDModifierManager::FilterTileModifiers()
{
	return FilterByTag(FilterStartpoint, UD_MODIFIER_TAG_TYPE_TILE);
}

FUDModifierPresentation UUDModifierManager::GetSpecified(int32 modifierId)
{
	return Modifiers[modifierId]->GetPresentation();
}

TArray<FUDModifierPresentation> UUDModifierManager::FilterByTag(const TArray<FUDModifierPresentation>& selection, int32 tag)
{
	TArray<FUDModifierPresentation> filtered = { };
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

#pragma region Modifier Creation API

void UUDModifierManager::CreateTileModifier(const TObjectPtr<UUDTileState>& tile, FUDModifierData newModifier)
{
	tile->Modifiers.Add(newModifier);
}

void UUDModifierManager::CreateFactionModifier(const TObjectPtr<UUDFactionState>& faction, FUDModifierData newModifier)
{
	faction->Modifiers.Add(newModifier);
}

void UUDModifierManager::RemoveTileModifier(const TObjectPtr<UUDTileState>& tile, int32 actionUniqueId)
{
	// Find & Delete
	const auto& found = tile->Modifiers.RemoveAll(
		[&actionUniqueId](const FUDModifierData& item) { return item.ActionUniqueId == actionUniqueId; }
	);
}

void UUDModifierManager::RemoveFactionModifier(const TObjectPtr<UUDFactionState>& faction, int32 actionUniqueId)
{
	// Find & Delete
	const auto& found = faction->Modifiers.RemoveAll(
		[&actionUniqueId](const FUDModifierData& item) { return item.ActionUniqueId == actionUniqueId; }
	);
}

void UUDModifierManager::RemoveAllTileModifiersOfType(const TObjectPtr<UUDTileState>& tile, int32 modifierTypeId)
{	
	// Find & Delete
	const auto& found = tile->Modifiers.RemoveAll(
		[&modifierTypeId](const FUDModifierData& item) { return item.ModifierTypeId == modifierTypeId; }
	);
}

void UUDModifierManager::RemoveAllFactionModifiersOfType(const TObjectPtr<UUDFactionState>& faction, int32 modifierTypeId)
{
	// Find & Delete
	const auto& found = faction->Modifiers.RemoveAll(
		[&modifierTypeId](const FUDModifierData& item) { return item.ModifierTypeId == modifierTypeId; }
	);
}

bool UUDModifierManager::HasTileModifierOfTypeTarget(const TObjectPtr<UUDTileState>& tile, FUDModifierData searchedModifier) const
{	
	// Find
	const auto& found = tile->Modifiers.FindByPredicate(
		[&searchedModifier](const FUDModifierData& item) { return item.IsTypeTargetEqual(searchedModifier); }
	);
	if (found)
	{
		return true;
	}
	return false;
}

bool UUDModifierManager::HasValueEqualTileModifier(const TObjectPtr<UUDTileState>& tile, FUDModifierData searchedModifier) const
{	
	// Find
	const auto& found = tile->Modifiers.FindByPredicate(
		[&searchedModifier](const FUDModifierData& item) { return item.IsValueEqual(searchedModifier); }
	);
	if (found)
	{
		return true;
	}
	return false;
}

int32 UUDModifierManager::GetFactionModifierActionId(const TObjectPtr<UUDFactionState>& faction, FUDModifierData searchedModifier) const
{
	// Find
	const auto& found = faction->Modifiers.FindByPredicate(
		[&searchedModifier](const FUDModifierData& item) { return item.IsTypeTargetEqual(searchedModifier); }
	);
	if (found)
	{
		return found->ActionUniqueId;
	}
	return UUDGlobalData::InvalidActionId;
}

bool UUDModifierManager::HasFactionModifierOfTypeTarget(const TObjectPtr<UUDFactionState>& faction, FUDModifierData searchedModifier) const
{	
	// Find
	const auto& found = faction->Modifiers.FindByPredicate(
		[&searchedModifier](const FUDModifierData& item) { return item.IsTypeTargetEqual(searchedModifier); }
	);
	if (found)
	{
		return true;
	}
	return false;
}

bool UUDModifierManager::HasValueEqualFactionModifier(const TObjectPtr<UUDFactionState>& faction, FUDModifierData searchedModifier) const
{	
	// Find
	const auto& found = faction->Modifiers.FindByPredicate(
		[&searchedModifier](const FUDModifierData& item) { return item.IsValueEqual(searchedModifier); }
	);
	if (found)
	{
		return true;
	}
	return false;
}

TArray<FUDModifierData> UUDModifierManager::GetAllFactionModifiers(const TObjectPtr<UUDFactionState>& faction, int32 modifierTypeId)
{
	TArray<FUDModifierData> list = { };

	for (const auto& modifier : faction->Modifiers)
	{
		if (modifier.ModifierTypeId == modifierTypeId)
		{
			list.Add(modifier);
		}
	}
	return list;
}

TArray<FUDModifierData> UUDModifierManager::GetAllTileModifiers(const TObjectPtr<UUDTileState>& tile, int32 modifierTypeId)
{
	TArray<FUDModifierData> list = { };

	for (const auto& modifier : tile->Modifiers)
	{
		if (modifier.ModifierTypeId == modifierTypeId)
		{
			list.Add(modifier);
		}
	}
	return list;
}

#pragma endregion