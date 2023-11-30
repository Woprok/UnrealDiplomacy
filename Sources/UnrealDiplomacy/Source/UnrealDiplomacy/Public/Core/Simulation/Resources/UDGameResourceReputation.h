// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGameResource.h"
#include "UDGameResourceReputation.generated.h"

/**
 * Definition for reputation. 
 * Most important resource in the game.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameResourceReputation : public UUDGameResource
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ResourceId; };
	virtual FUDResourcePresentation GetPresentation() const override;
	virtual int32 GetFactionStartingAmount() const override { return FactionStartingAmount; };
	virtual int32 GetTileStartingAmount() const override { return TileStartingAmount; };
public:
	static const int32 ResourceId = 1001;
	UPROPERTY()
	int32 FactionStartingAmount = 0;
	UPROPERTY()
	int32 TileStartingAmount = 0;
};