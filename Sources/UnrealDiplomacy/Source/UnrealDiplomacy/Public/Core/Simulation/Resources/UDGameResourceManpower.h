// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGameResource.h"
#include "UDGameResourceManpower.generated.h"

/**
 * Definition for manpower.
 * Primarily used for resolving faction conflicts.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameResourceManpower : public UUDGameResource
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceId; };
	virtual FUDResourcePresentation GetPresentation() const override;
public:
	static const int32 ResourceId = 1005;
};