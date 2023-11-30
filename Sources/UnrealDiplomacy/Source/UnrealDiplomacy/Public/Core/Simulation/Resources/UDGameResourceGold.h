// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGameResource.h"
#include "UDGameResourceGold.generated.h"

/**
 * Definition for gold.
 * Useable to solve most problems.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameResourceGold : public UUDGameResource
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceId; };
	virtual FUDResourcePresentation GetPresentation() const override;
	virtual int32 GetFactionStartingAmount() const override { return FactionStartingAmount; };
	virtual int32 GetTileStartingAmount() const override { return TileStartingAmount; };
public:
	static const int32 ResourceId = 1002;
	UPROPERTY()
	int32 FactionStartingAmount = 50;
	UPROPERTY()
	int32 TileStartingAmount = 0;
};