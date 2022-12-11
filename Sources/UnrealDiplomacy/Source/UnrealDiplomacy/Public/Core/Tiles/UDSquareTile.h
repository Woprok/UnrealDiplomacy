// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDActor.h"
#include "UDSquareTile.generated.h"

/** 
 * Determines how can player interact with the tile.
 */
UENUM(BlueprintType)
enum class ESquareTileType : uint8
{
	// Undefined
	INVALID,
	// Defined, but kept secret
	HIDDEN,
	// Visible, without any interactable promise
	VISIBLE,
	// Visible and can be interacted with
	INTERACTABLE,
	MAX UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSquareTile : public AUDActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	ESquareTileType TileType;
};