// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGameResource.h"
#include "UDGameResourceFood.generated.h"

/**
 * Definition for food.
 * Used to create more "living creatures".
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameResourceFood : public UUDGameResource
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceId; };
	virtual FUDResourcePresentation GetPresentation() const override;
public:
	static const int32 ResourceId = 1003;
};