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

	RegisterCoreModifiers();
	RegisterAdditionalModifiers();
}

void UUDModifierManager::RegisterCoreModifiers()
{
	RegisterModifierList(UUDModifierDatabase::GetDefaultModifiers(this));
	RegisterModifierList(UUDModifierDatabase::GetTileModifiers(this));
	RegisterModifierList(UUDModifierDatabase::GetFactionModifiers(this));
}

void UUDModifierManager::RegisterAdditionalModifiers()
{
	// This is only for inherited managers.
	return;
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
	// Find
	const auto& found = tile->Modifiers.FindByPredicate(
		[&actionUniqueId](const FUDModifierData& item) { return item.ActionUniqueId == actionUniqueId; }
	);
	// Delete
	if (found)
	{
		tile->Modifiers.Remove(*found);
	}
}

void UUDModifierManager::RemoveFactionModifier(const TObjectPtr<UUDFactionState>& faction, int32 actionUniqueId)
{
	// Find
	const auto& found = faction->Modifiers.FindByPredicate(
		[&actionUniqueId](const FUDModifierData& item) { return item.ActionUniqueId == actionUniqueId; }
	);
	// Delete
	if (found)
	{
		faction->Modifiers.Remove(*found);
	}
}

bool UUDModifierManager::HasTileModifier(const TObjectPtr<UUDTileState>& tile, FUDModifierData searchedModifier) const
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

bool UUDModifierManager::HasFactionModifier(const TObjectPtr<UUDFactionState>& faction, FUDModifierData searchedModifier) const
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

TArray<FUDModifierData> UUDModifierManager::GetAllFactionModifiers(const TObjectPtr<UUDFactionState>& faction, int32 modifierId)
{
	TArray<FUDModifierData> list = { };

	for (const auto& modifier : faction->Modifiers)
	{
		if (modifier.ModifierTypeId == modifierId) 
		{
			list.Add(modifier);
		}
	}
	return list;
}

#pragma endregion