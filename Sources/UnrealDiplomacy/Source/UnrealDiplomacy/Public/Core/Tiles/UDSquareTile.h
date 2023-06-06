// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDSquareTile.generated.h"

/** 
 * Determines if player can interact with the tile.
 */
UENUM(BlueprintType)
enum class ESquareTileType : uint8
{
	/**
	 * Undefined.
	 */
	INVALID,
	/**
	 * Defined, but is not visible to player.
	 */
	HIDDEN,
	/**
	 * Defined and Visible, but interaction is disabled.
	 */
	VISIBLE,
	/**
	 * Defined, visible and interaction is allowed.
	 */
	INTERACTABLE,
	MAX UMETA(Hidden)
};

DECLARE_DELEGATE_OneParam(TileSelectedDelegate, TObjectPtr<AUDSquareTile>);

/**
 * Tile that is placeable on the map and represent's part of the game board.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSquareTile : public AUDActor
{
	GENERATED_BODY()
public:
	AUDSquareTile();
	/**
	 * Position inside of the grid.
	 */
	UFUNCTION()
	void SetTilePosition(FIntPoint tilePosition);
	/**
	 * Retrieves position of this tile inside of the grid.
	 */
	UFUNCTION(BlueprintCallable)
	FIntPoint GetTilePosition();
	/**
	 * Called after notifying UI for any tile subscriber about tile being selected.
	 */
	UFUNCTION(BlueprintCallable)
	void NotifyTileSelected();
	/**
	 * Grid invoked event on the tile. Tile is responsible for visually updating itself.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Tile")
	void OnUpdate();
	/**
	 * Invoked when this tile is selected.
	 * Grid is handling what it means to be selected.
	 */
	TileSelectedDelegate Selected;
public:
	/** 
	 * Event when this actor is clicked by the mouse when using the clickable interface. 
	 */
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	ESquareTileType TileType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	TObjectPtr<UStaticMeshComponent> TileMesh;
private:
	UPROPERTY()
	FIntPoint TilePosition;
};