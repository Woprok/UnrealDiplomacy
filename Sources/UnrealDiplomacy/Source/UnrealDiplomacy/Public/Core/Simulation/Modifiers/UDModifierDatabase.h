// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModifierDatabase.generated.h"

// Forward Declarations

class IUDModifierInterface;

UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDModifierDatabase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Returns new instances of default modifiers.
	 */
	static TArray<TScriptInterface<IUDModifierInterface>> GetDefaultModifiers(UObject* parent);
	/**
	 * Returns new instances of tile modifiers.
	 */
	static TArray<TScriptInterface<IUDModifierInterface>> GetTileModifiers(UObject* parent);
	/**
	 * Returns new instances of faction modifiers.
	 */
	static TArray<TScriptInterface<IUDModifierInterface>> GetFactionModifiers(UObject* parent);
};