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
	virtual int32 GetFactionStartingAmount() const override { return FactionStartingAmount; };
	virtual int32 GetTileStartingAmount() const override { return TileStartingAmount; };
	virtual int32 GetTileWeight() const override { return TileWeight; };
	virtual UMaterialInterface* GetIcon() const override { return Icon; };
public:
	static const int32 ResourceId = 1003;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 FactionStartingAmount = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileStartingAmount = 800;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileWeight = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	int32 TileUpkeep = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon Config")
	UMaterialInterface* Icon;
};