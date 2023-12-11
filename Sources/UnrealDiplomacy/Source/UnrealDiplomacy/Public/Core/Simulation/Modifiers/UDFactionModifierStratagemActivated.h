// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDFactionModifier.h"
#include "UDFactionModifierStratagemActivated.generated.h"

/**
 * Marks stratagem as used on this faction.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionModifierStratagemActivated : public UUDFactionModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 1004;
};