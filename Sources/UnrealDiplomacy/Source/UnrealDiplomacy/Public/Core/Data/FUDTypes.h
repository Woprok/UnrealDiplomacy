// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "FUDTypes.generated.h"


class UNREALDIPLOMACY_API FUDTypes
{
public:
	FUDTypes();
	~FUDTypes();
};

UENUM(BlueprintType)
enum class EHexTileType : uint8
{
	INVALID,
	GRASS,
	WATER,
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
struct UNREALDIPLOMACY_API FUDExtension
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
	// Single region
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
	FUDTileRegion Region;

	// Single biome
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tile)
		FUDTileBiome Biome;
	
	// Any amount of features [Ocean, River, Hill, Beach]
	TArray<FUDTileFeature> Features;


	// Any amount of links to extensions ?
};