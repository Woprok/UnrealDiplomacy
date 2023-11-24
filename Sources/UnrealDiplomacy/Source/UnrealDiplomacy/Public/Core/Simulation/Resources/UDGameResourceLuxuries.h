// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGameResource.h"
#include "UDGameResourceLuxuries.generated.h"

/**
 * Definition for luxuries.
 * Used for anything.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameResourceLuxuries : public UUDGameResource
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceId; };
	virtual FUDResourcePresentation GetPresentation() const override;
public:
	static const int32 ResourceId = 1006;
};