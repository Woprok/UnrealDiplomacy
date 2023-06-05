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
	 * Returns new instances of system of actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetSystemActions(UObject* parent);
	/**
	 * Returns new instances of gaia of actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetGaiaActions(UObject* parent);
	/**
	 * Returns new instances of game of actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetGameActions(UObject* parent);
	/**
	 * Returns new instances of deal of actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetDealActions(UObject* parent);
	/**
	 * Returns new instances of setting of actions.
	 */
	static TArray<TScriptInterface<IUDActionInterface>> GetSettingActions(UObject* parent);
};