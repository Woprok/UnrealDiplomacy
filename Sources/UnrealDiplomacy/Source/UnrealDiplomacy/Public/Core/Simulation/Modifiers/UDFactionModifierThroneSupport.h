// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDFactionModifier.h"
#include "UDFactionModifierThroneSupport.generated.h"

/**
 * Creator supports target ancesion to the throne.
 * Sharing his reputation with the target.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionModifierThroneSupport : public UUDFactionModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 1001;
};