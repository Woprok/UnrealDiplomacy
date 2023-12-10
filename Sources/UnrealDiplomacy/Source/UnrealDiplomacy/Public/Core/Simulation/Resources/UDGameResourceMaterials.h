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
	virtual int32 GetFactionStartingAmount() const override { return FactionStartingAmount; };
	virtual int32 GetTileStartingAmount() const override { return TileStartingAmount; };
	virtual int32 GetTileWeight() const override { return TileWeight; };
	virtual UMaterialInterface* GetIcon() const override { return Icon; };
public:
	static const int32 ResourceId = 1004;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 FactionStartingAmount = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileStartingAmount = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileWeight = 75;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon Config")
	UMaterialInterface* Icon;
};