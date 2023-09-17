// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "UDDefaultModifier.generated.h"

/**
 * Default modifier that serves as fallback.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDefaultModifier : public UObject, public IUDModifierInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
public:
	static const int32 ModifierTypeId = 0;
};