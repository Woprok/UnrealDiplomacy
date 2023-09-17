// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Modifiers/UDDefaultModifier.h"
#include "UDDefaultModifierInvalid.generated.h"

/**
 * Default invalid modifier that serves as fallback.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDefaultModifierInvalid : public UUDDefaultModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override;
};