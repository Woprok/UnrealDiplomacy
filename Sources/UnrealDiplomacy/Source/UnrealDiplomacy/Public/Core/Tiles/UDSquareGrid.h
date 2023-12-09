// Copyright Miroslav Valach
// TODO added a way to give each tile a different spawn chance, spawn rules etc...

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDSquareGrid.generated.h"

// Forward Declarations

class UUDActionAdministrator;
class AUDSquareTile;
class UUDMapState;
class UUDTileState;
struct FUDActionData;

/**
 * Tile type class mapping to type Id.
 * e.g. ResourceType is binded in this version to Type on TileState. 
 * Thus this will link type to a blueprint class, that will be spawned on the map.
 */
USTRUCT(Blueprintable, BlueprintType)
struct FUDTileType
{
	GENERATED_BODY()
public:
	FUDTileType() {};
	UPROPERTY(EditAnywhere)
	FName TypeName;
	UPROPERTY(EditAnywhere)
	int32 TypeId;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AUDSquareTile> TileType;
};

/**
 * Grid that uses square shaped tiles.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSquareGrid : public AUDActor
{
	GENERATED_BODY()
public:
	/** Primary call for creating the world, once Authority is defined. */
	UFUNCTION(BlueprintCallable)
	void GenerateWorld();
	/** Updates the world, to reflect current state. */
	UFUNCTION(BlueprintCallable)
	void UpdateWorld();
	/** Reference to model, used for retrieving all important action. */
	UFUNCTION(BlueprintCallable)
	void SetAuthority(UUDActionAdministrator* model);
	/** Public handle for notifying grid to update it's tiles based on changes in model. */
	UFUNCTION(BlueprintCallable)
	virtual void OnUpdate(const FUDActionData& action);
	UFUNCTION()
	virtual void OnSynchronized();
protected:
	/** Creates map from the map state. */
	void Create(TObjectPtr<UUDMapState> state);
	/** Creates map from the map state. */
	void Update(TObjectPtr<UUDMapState> state);
	/** Invoked by tiles, when they are selected by the user. */
	virtual void OnTileSelected(TObjectPtr<AUDSquareTile> newSelectedtile);
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> MapModel = nullptr;
	/** Updates faction colors. */
	void UpdateColorMap();
private:
	void DeleteTile(TObjectPtr<UUDTileState> dataTile);
	void CreateTile(TObjectPtr<UUDTileState> dataTile);
	FVector CalculateTileWorldPosition(FIntPoint tilePosition);
	TSubclassOf<AUDSquareTile> RetrieveTileType(TObjectPtr<UUDTileState> tileClass);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileLength = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileDistance = 102;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileHorizontalOffset = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileVerticalOffset = 100;
	
	/** Maps Type ID to specific Tile type class. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	TMap<int32, FUDTileType> TypeIdTileTypeMap;
	/** Fallback type for tiles to use in case they were not properly defined. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	TSubclassOf<AUDSquareTile> FallbackTileType;
private:
	/** Current map of tiles present in the world. */
	UPROPERTY()
	TMap<FIntPoint, TWeakObjectPtr<AUDSquareTile>> GridMap;
	/** Last selected tile, that should be updated on selection change. */
	UPROPERTY()
	TWeakObjectPtr<AUDSquareTile> LastSelectedTile;
	/** Stores faction id to faction base color vector, primarily for handling colors. */
	TMap<int32, FLinearColor> FactionColors = { };
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	TArray<FLinearColor> Colors = { FLinearColor(0.5,0.5,0.5,1.0) };
};