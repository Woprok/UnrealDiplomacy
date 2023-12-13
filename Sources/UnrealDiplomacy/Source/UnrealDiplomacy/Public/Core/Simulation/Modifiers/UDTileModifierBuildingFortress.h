// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDTileModifier.h"
#include "UDTileModifierBuildingFortress.generated.h"

/**
 * This tile has at least one building of type Fortress on it.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileModifierBuildingFortress : public UUDTileModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 2008;
};