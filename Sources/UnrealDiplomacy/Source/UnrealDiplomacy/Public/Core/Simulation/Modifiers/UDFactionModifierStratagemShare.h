// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDFactionModifier.h"
#include "UDFactionModifierStratagemShare.generated.h"

/**
 * Creator shares stratagem with the target.
 * Sharing his stratagem with the target.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionModifierStratagemShare : public UUDFactionModifier
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ModifierTypeId; };
	virtual FUDModifierPresentation GetPresentation() const override;
public:
	static const int32 ModifierTypeId = 1002;
};