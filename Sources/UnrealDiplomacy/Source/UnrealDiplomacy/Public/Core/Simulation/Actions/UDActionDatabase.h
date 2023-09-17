// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionDatabase.generated.h"

// Forward Declarations

class IUDActionInterface;

UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDActionDatabase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Returns new instances of default actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetDefaultActions(UObject* parent);
	/**
	 * Returns new instances of system actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetSystemActions(UObject* parent);
	/**
	 * Returns new instances of gaia actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetGaiaActions(UObject* parent);
	/**
	 * Returns new instances of game actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetGameActions(UObject* parent);
	/**
	 * Returns new instances of deal actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetDealActions(UObject* parent);
	/**
	 * Returns new instances of decision actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetDecisionActions(UObject* parent);
	/**
	 * Returns new instances of setting actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetSettingActions(UObject* parent);
};