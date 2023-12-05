// Copyright Miroslav Valach

#include "Core/Tiles/UDSquareTile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Skirmish/UDSkirmishHUD.h"

AUDSquareTile::AUDSquareTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SquareTileRoot"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquareTileMesh"));
	TileMesh->SetupAttachment(RootComponent);
}

void AUDSquareTile::SetTilePosition(FIntPoint tilePosition)
{
	TilePosition = tilePosition;
}

FIntPoint AUDSquareTile::GetTilePosition()
{
	return TilePosition;
}

int32 AUDSquareTile::GetTileType()
{
	return TileType;
}

void AUDSquareTile::SetTileType(int32 tileType)
{
	TileType = tileType;
}

void AUDSquareTile::NotifyTileSelected()
{
	Selected.ExecuteIfBound(this);
}

void AUDSquareTile::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	// call our handler on the HUD

	if (ButtonPressed == EKeys::LeftMouseButton)
	{
		AUDSkirmishHUD::Get(GetWorld())->RequestNotifyOnTileSelected(TilePosition);
		NotifyTileSelected();
	}
}