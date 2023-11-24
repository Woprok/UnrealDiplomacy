// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGameResource.h"
#include "UDGameResourceMaterials.generated.h"

/**
 * Definition for materials.
 * Used for construction.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameResourceMaterials : public UUDGameResource
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceId; };
	virtual FUDResourcePresentation GetPresentation() const override;
public:
	static const int32 ResourceId = 1004;
};