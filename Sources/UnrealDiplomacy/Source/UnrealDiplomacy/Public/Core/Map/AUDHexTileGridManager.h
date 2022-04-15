// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AUDHexTileGridManager.generated.h"

struct FUDTileRow;
class AUDHexTile;

UCLASS()
class UNREALDIPLOMACY_API AUDHexTileGridManager : public AActor
{
	GENERATED_BODY()	
protected:
	// Internal array that holds all spawned actors. First is row, second is column.
	TArray<TArray<TObjectPtr<AUDHexTile>>> HexGrid2DArray;
	// Radius should be based on mesh size of actor that is provided in setup.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationLayout")
	float TileRadius = 100;
	// Extended radius is used to determine spacing between tiles.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationLayout")
	float TileExtendedRadius = 104;
	// Automatically assigned value during map gen.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationLayout")
	float TileHorizontalOffsetEven;
	// Automatically assigned value during map gen.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationLayout")
	float TileHorizontalOffsetOdd;
	// Automatically assigned value during map gen.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationLayout")
	float TileVerticalOffset;
	// Defines random seed for map to create different results with same parameters.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationConfiguration")
	int32 MapSeed;
	// Defines amount of players that is expected to be used in parameters of map generation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationConfiguration")
	int PlayerCount = 1;
	// Customizable width to create larger maps for maps that would otherwise be smaller due to parameters.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationConfiguration")
	int32 MinimumGridWidth = 3;
	// Customizable height to create larger maps for maps that would otherwise be smaller due to parameters.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationConfiguration")
	int32 MinimumGridHeight = 3;
	// Radius that determines amount of tiles excluded around each player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationConfiguration")
	int32 PlayerExclusionRadius = 1;
	// Additional radius that is marked as buffer zone between players during generation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationConfiguration")
	int32 PlayerBufferRadius = 1;
	// Extension of the map in width and height at both sides to create border visually.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationConfiguration")
	int32 BorderTileThickness = 1;	
	// Automatically assigned value during map gen. It's 1 + amount of tiles required by exclusion radius.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationParameters")
	int32 PlayerExclusionTiles = 7;
	// Automatically assigned value during map gen. It's PlayerExclusionRadius + amount of tiles required by buffer radius.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationParameters")
	int32 PlayerBufferTiles = 19;
	// Automatically assigned value during map gen. Excludes BorderTileThickness.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationParameters")
	int32 UseableGridWidth = 3;
	// Automatically assigned value during map gen. Excludes BorderTileThickness. 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationParameters")
	int32 UseableGridHeight = 3;
	// Automatically assigned value during map gen. Includes BorderTileThickness. Should not be used for anything.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationParameters")
	int32 FullGridWidth = 5;
	// Automatically assigned value during map gen. Includes BorderTileThickness. Should not be used for anything.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HexGrid|GenerationParameters")
	int32 FullGridHeight = 5;

	// Tiles used for world, does not carry data, but holds reference to indices in data array.
	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	TSubclassOf<AUDHexTile> MapWorldHexTile;
	// Tile used for border, does not carry or read data. 
	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	TSubclassOf<AUDHexTile> MapBorderHexTile;
	
public:
	// Sets default values for this actor's properties
	AUDHexTileGridManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Prepares map generation parameters, adjusting any parameter that would cause fatal problem. Follows strict step order.
	void PrepareMapParameters();	
	// Updates TileOffsets based on TileRadius
	void DefineTileOffsets();
	// Updates Player tile counts to fit into exclusion and border zones.
	void DefinePlayerTileCounts();
	// Calculate useable grid size required.
	void DefineGridSize();
	// Updates FullGrid size based on BorderTileThickness.
	void DefineFullGridSize();
	// Creates empty array for actors that will be spawned.
	void InitializeGridArray();
	
	// Helper to get proper place in world for a tile. Takes indices x and y of an grid array.
	FIntPoint DetermineTilePositionInWorld(const int32 x, const int32 y);
	// Helper to spawn tile in world.
	void SpawnTile(const int32 xWorld, const int32 yWorld, TSubclassOf<AUDHexTile> tileType, int32 xData, int32 yData);
	// Direct spawn of border tile.
	void SpawnBorderTile(int32 x, int32 y);
	
	// Creates map border with defined thickness.	
	UFUNCTION(BlueprintCallable)
	void GenerateMapBorder();
	// Creates actors for provided map within predefined grid.
	//UFUNCTION(BlueprintCallable)
	//void GenerateFromTileData(TArray<FUDTileRow>& map);
};
