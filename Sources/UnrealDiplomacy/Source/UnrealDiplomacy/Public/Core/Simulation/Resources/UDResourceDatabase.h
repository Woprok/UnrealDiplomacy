// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDResourceDatabase.generated.h"

// Forward Declarations

class IUDResourceInterface;

UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDResourceDatabase : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Returns new instances of default resources.
	 */
	static TArray<TScriptInterface<IUDResourceInterface>> GetDefaultResources(UObject* parent);
	/**
	 * Returns new instances of default resources.
	 */
	static TArray<TScriptInterface<IUDResourceInterface>> GetGameResources(UObject* parent);
};