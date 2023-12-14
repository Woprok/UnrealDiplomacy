// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDFactionModifier.h"
#include "UDFactionModifierThroneUsurper.generated.h"

/**
 * This modifiers prevent repeated usurption of the throne.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionModifierThroneUsurper : public UUDFactionModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 1005;
};