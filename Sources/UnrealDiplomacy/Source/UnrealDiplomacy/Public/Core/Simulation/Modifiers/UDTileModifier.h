// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "UDTileModifier.generated.h"

/**
 * Generic ancestor for all tile modifiers.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileModifier : public UObject, public IUDModifierInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
public:
	static const int32 ModifierTypeId = 2000;
};