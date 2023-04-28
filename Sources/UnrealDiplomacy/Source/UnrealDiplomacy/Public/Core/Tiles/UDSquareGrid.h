// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDSquareTile.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDSquareGrid.generated.h"

/**
 * Grid that uses square shaped tiles.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSquareGrid : public AUDActor
{
	GENERATED_BODY()
public:
	/**
	 * Primary call for creating the world, once Authority is defined.
	 */
	UFUNCTION(BlueprintCallable)
	void GenerateWorld()
	{
		Create(MapModel->GetMapState());
	}
	/**
	 * Reference to model, used for retrieving all important action.
	 */
	UFUNCTION(BlueprintCallable)
	void SetAuthority(UUDActionAdministrator* model)
	{
		MapModel = model;
	}
	/**
	 * Public handle for notifying grid to update it's tiles based on changes in model.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void OnUpdate()
	{
		if (!MapModel->IsMapStateReady())
		{
			UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map not yet present."));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map ready."));
			if (Grid.Num() > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map updated."));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map generated."));
				GenerateWorld();
			}
		}
	}
	UFUNCTION(BlueprintCallable)
	virtual void EmplaceTypeMapping(int32 key, int32 target)
	{
		TypeToClassMapping.Emplace(key, target);
	}
protected:
	/**
	 * Creates map from the map state.
	 */
	void Create(TObjectPtr<UUDMapState> state)
	{
		for (TObjectPtr<UUDTileState> dataTile : state->Tiles)
		{
			// Spawn new tile.
			CreateTile(dataTile);
			// Invoke update.
			Grid.Last()->OnUpdate();
		}
	}
	/**
	 * Invoked by tiles, when they are selected by the user. 
	 */
	virtual void OnTileSelected(TObjectPtr<AUDSquareTile> tile)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Selected[%d][%d]."), tile->GetTilePosition().X, tile->GetTilePosition().Y);
	}
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> MapModel = nullptr;
private:
	void CreateTile(TObjectPtr<UUDTileState> dataTile)
	{
		FVector worldPosition = CalculateTileWorldPosition(dataTile->Position);
		TSubclassOf<AUDSquareTile> tileClass = RetrieveTileType(dataTile);
		TObjectPtr<AUDSquareTile> newWorldTile = GetWorld()->SpawnActor<AUDSquareTile>(tileClass, worldPosition, FRotator::ZeroRotator);
		newWorldTile->SetTilePosition(dataTile->Position);
		newWorldTile->Selected.BindUObject(this, &AUDSquareGrid::OnTileSelected);
		Grid.Add(newWorldTile);
	}
	FVector CalculateTileWorldPosition(FIntPoint tilePosition)
	{
		float x = tilePosition.X * TileHorizontalOffset;
		float y = tilePosition.Y * TileVerticalOffset;
		float z = 0;
		return FVector(x, y, z);
	}
	TSubclassOf<AUDSquareTile> RetrieveTileType(TObjectPtr<UUDTileState> tileClass)
	{
		if (TypeToClassMapping.Contains(tileClass->Type))
		{
			// Returns value in TileTypes based on index mapping of TypeMapping.
			auto tmp = TypeToClassMapping[tileClass->Type];
			return TileTypeClasses[tmp];
		}
		// Fallback...
		return TileType;
	}
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileLength = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileDistance = 104;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileHorizontalOffset = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	float TileVerticalOffset = 100;
	// TODO added a way to give each tile a different spawn chance, spawn rules etc...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	TArray<TSubclassOf<AUDSquareTile>> TileTypeClasses;
	/**
	 * Fallback type for tiles to use.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SquareGrid|Config")
	TSubclassOf<AUDSquareTile> TileType;
	/**
	 * TODO proper tile mapping feature...
	 */
	TMap<int32, int32> TypeToClassMapping;
private:
	/**
	 * Current list of tiles present in the world.
	 */
	UPROPERTY()
	TArray<TWeakObjectPtr<AUDSquareTile>> Grid;
};