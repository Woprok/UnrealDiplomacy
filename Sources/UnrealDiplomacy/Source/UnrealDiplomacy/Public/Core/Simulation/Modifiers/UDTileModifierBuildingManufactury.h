// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDTileModifier.h"
#include "UDTileModifierBuildingManufactury.generated.h"

/**
 * This tile has at least one building of type Manufactury on it.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileModifierBuildingManufactury : public UUDTileModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 2006;
};