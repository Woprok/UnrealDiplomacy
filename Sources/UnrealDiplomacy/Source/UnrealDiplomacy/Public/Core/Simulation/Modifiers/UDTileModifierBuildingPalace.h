// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDTileModifier.h"
#include "UDTileModifierBuildingPalace.generated.h"

/**
 * This tile has at least one building of type Palace on it.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileModifierBuildingPalace : public UUDTileModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 2007;
};