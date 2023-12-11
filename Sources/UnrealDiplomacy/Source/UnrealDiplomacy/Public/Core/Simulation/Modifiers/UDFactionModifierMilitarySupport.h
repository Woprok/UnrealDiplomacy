// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDFactionModifier.h"
#include "UDFactionModifierMilitarySupport.generated.h"

/**
 * Creator supports target army with his own.
 * Sharing his manpower with the target.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionModifierMilitarySupport : public UUDFactionModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 1003;
};