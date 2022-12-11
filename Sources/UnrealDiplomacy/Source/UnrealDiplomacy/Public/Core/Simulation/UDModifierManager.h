// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDWorldState.h"
#include "UDModifier.h"
#include "UDModifierManager.generated.h"

/**
 * Contains method for working with modifiers and aggregating them.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDModifierManager : public UObject
{
	GENERATED_BODY()
public:
	void ApplyTileModifier(TObjectPtr<UUDTileState> tileState, TObjectPtr<UUDModifier> modifier) 
	{
		tileState->Modifiers.Add(modifier);
	}

	void RemoveTileModifier(TObjectPtr<UUDTileState> tileState, int32 actionDataUniqueId)
	{
		// requires to be found first, list should be always short.
		auto removedData = *tileState->Modifiers.FindByPredicate(
			[actionDataUniqueId](TObjectPtr<UUDModifier> modifier) 
			{ 
				return modifier->GetAssociatedActionUniqueId() == actionDataUniqueId;
			}
		);
		tileState->Modifiers.Remove(removedData);
	}

	TArray<TObjectPtr<UUDModifier>> GetTileModifiers(TObjectPtr<UUDTileState> tileState, int32 modifierTypeId)
	{
		TArray<TObjectPtr<UUDModifier>> searchedModifiers;
		for (auto modifier : tileState->Modifiers)
		{
			if (modifier->GetModifierTypeId() == modifierTypeId)
			{
				searchedModifiers.Add(modifier);
			}
		}
		return searchedModifiers;
	}
};