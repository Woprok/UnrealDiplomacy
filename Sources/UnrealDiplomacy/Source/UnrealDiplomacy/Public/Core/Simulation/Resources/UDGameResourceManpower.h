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
	virtual int32 GetFactionStartingAmount() const override { return FactionStartingAmount; };
	virtual int32 GetTileStartingAmount() const override { return TileStartingAmount; };
	virtual int32 GetTileWeight() const override { return TileWeight; };
	virtual UMaterialInterface* GetIcon() const override { return Icon; };
public:
	static const int32 ResourceId = 1005;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 FactionStartingAmount = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileStartingAmount = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileWeight = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileUpkeep = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon Config")
	UMaterialInterface* Icon;
};