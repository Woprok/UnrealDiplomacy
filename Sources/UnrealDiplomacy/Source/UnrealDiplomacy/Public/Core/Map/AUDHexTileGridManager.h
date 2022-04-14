// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AUDHexTileGridManager.generated.h"

class AUDHexTile;

UCLASS()
class UNREALDIPLOMACY_API AUDHexTileGridManager : public AActor
{
	GENERATED_BODY()
	
protected:
	TArray<TArray<TObjectPtr<AUDHexTile>>> HexGrid2DArray;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Layout")
	float TileRadius = 100;

	UPROPERTY(VisibleAnywhere, Category = "HexGrid|Layout")
	float TileHorizontalOffset;

	UPROPERTY(VisibleAnywhere, Category = "HexGrid|Layout")
	float OddRowTileHorizontalOffset;

	UPROPERTY(VisibleAnywhere, Category = "HexGrid|Layout")
	float TileVerticalOffset;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	TSubclassOf<AUDHexTile> GrassHexTile;

	UPROPERTY(EditAnywhere, Category = "HexGrid|Setup")
	TSubclassOf<AUDHexTile> WaterHexTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationParameters")
	int MapSeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationParameters")
	int32 GridWidth = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationParameters")
	int32 GridHeight = 3;

	// Based on this setting, grid will be forced to be larger, if players exclusion area cant be reached.
	// 1 is one tile exclusion area, player will have 7 tiles for himself at start.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationParameters")
	int32 PlayerExclusionRadius = 1;
	
	// During generation add larger area to exclusion in each step, to ensure existence of exclusion radius.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationParameters")
	int32 GenerationBufferRadius = 1;
	
	// Determines impassable and unselectable border around the map. Add on top of the dimensions.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HexGrid|GenerationParameters")
	int32 BorderTileThickness = 1;
	
public:
	// Sets default values for this actor's properties
	AUDHexTileGridManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Generate();

	int32 GetTilesPerPlayer();
	void AdjustGridSize();
	void GenerateMapInSteps();
	void GenerateBorder();
	
	void SetupPositionConstantValues();
	void SetupMapSize();
	void GenerateMap();
};
