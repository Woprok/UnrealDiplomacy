// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "UDFactionModifier.generated.h"

/**
 * Generic ancestor for all faction modifiers.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionModifier : public UObject, public IUDModifierInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
public:
	static const int32 ModifierTypeId = 1000;
};