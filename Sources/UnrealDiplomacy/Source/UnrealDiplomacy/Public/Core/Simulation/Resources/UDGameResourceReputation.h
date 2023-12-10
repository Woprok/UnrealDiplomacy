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
	virtual int32 GetTileWeight() const override { return TileWeight; };
	virtual UMaterialInterface* GetIcon() const override { return Icon;  };
public:
	static const int32 ResourceId = 1001;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 FactionStartingAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileStartingAmount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileWeight= 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon Config")
	UMaterialInterface* Icon;
};