// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "UDDefaultModifierInvalid.generated.h"

/**
 * Default invalid modifier that serves as fallback.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDefaultModifierInvalid : public UObject, public IUDModifierInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override;
};