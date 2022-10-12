// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "UDTypes.generated.h"


class UNREALDIPLOMACY_API FUDTypes
{
public:
	FUDTypes();
	~FUDTypes();
};

// Each tile is either part of world or border around the world. This can be used to determine additional behaviour.
UENUM(BlueprintType)
enum class EHexTileType : uint8
{
	INVALID,
	BORDER,
	WORLD,
	MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDResource : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
	int32 Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
	int32 Minimum;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
	int32 Maximum;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
	int32 Gain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
	TObjectPtr<UTexture> Icon;
};

// Representation of tile most important part, be it Underground, Ground or Sky...
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDTileRegion : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Region)
	FString Name;
};

// Represents primary weather, fauna, flora condition of a tile such as Desert, Temperate, Tropical, Alpine, Tundra...
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDTileBiome : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Biome)
	FString Name;
};

// Represents various features such as River, Lake, Ocean, Lava Lake, Volcano, Mountain, Hill, Canyon, Ravine, Forest, Jungle, Plain...
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDTileFeature : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Feature)
	FString Name;
};

// Represents various special features add to Tile such as Settlement, Capital City, Improvements...
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDTileExtension : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Feature)
	FString Name;
};

USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDTile
{
	GENERATED_BODY()
public:
	// Owner Unique Id.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	int32 Owner;
	
	// Defines position in the world.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tile)
	FIntPoint WorldPosition;
	
	// Single region.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	FUDTileRegion Region;

	// Single biome.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	FUDTileBiome Biome;
	
	// Any amount of features [Ocean, River, Hill, Beach]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	TArray<FUDTileFeature> Features;
	
	// Any amount of links to extensions ?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	TArray<FUDTileExtension> Extensions;
};

USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDTileRow
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	TArray<FUDTile> Tiles;
};