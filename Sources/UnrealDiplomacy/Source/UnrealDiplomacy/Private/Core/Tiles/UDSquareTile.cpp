// Copyright Miroslav Valach

#include "Core/Tiles/UDSquareTile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AUDSquareTile::AUDSquareTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SquareTileRoot"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquareTileMesh"));
	TileMesh->SetupAttachment(RootComponent);
}