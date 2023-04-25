// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGlobalData.generated.h"

/**
 * Collection of constants that are used as special values across the project.
 */
UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDGlobalData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * This Id is always considered invalid for all operations.
	 */
	static const int32 InvalidActionId = -1;
};