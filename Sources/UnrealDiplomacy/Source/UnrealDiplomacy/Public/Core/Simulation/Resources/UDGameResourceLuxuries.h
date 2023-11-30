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
	virtual int32 GetFactionStartingAmount() const override { return FactionStartingAmount; };
	virtual int32 GetTileStartingAmount() const override { return TileStartingAmount; };
	virtual int32 GetTileWeight() const override { return TileWeight; };
public:
	static const int32 ResourceId = 1006;
	UPROPERTY()
	int32 FactionStartingAmount = 10;
	UPROPERTY()
	int32 TileStartingAmount = 40;
	UPROPERTY()
	int32 TileWeight = 10;
};