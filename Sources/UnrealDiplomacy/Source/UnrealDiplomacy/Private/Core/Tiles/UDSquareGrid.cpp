// Copyright Miroslav Valach

#include "Core/Tiles/UDSquareGrid.h"
#include "Core/Tiles/UDSquareTile.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/UDGlobalData.h"

void AUDSquareGrid::GenerateWorld()
{
	Create(MapModel->GetMapState());
}

void AUDSquareGrid::UpdateWorld()
{
	Update(MapModel->GetMapState());
}

void AUDSquareGrid::SetAuthority(UUDActionAdministrator* model)
{
	MapModel = model;

	TObjectPtr<AUDSkirmishPlayerController> pc = AUDSkirmishPlayerController::Get(GetWorld());
	// TODO VERIFY THAT THIS IS NEVER AND WILL NEVER BE CALLED TWICE
	pc->OnSynchronizationFinishedEvent.AddUniqueDynamic(this, &AUDSquareGrid::OnSynchronized);
	pc->OnWorldSimulationUpdatedEvent.AddUniqueDynamic(this, &AUDSquareGrid::OnUpdate);
}

void AUDSquareGrid::OnUpdate(const FUDActionData& action)
{
	OnSynchronized();
}

void AUDSquareGrid::OnSynchronized()
{
	UpdateColorMap();
	if (!MapModel->IsMapStatePresent())
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map not yet present."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map ready."));
		if (GridMap.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map updated."));
			UpdateWorld();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Map generated."));
			GenerateWorld();
		}
	}
}

void AUDSquareGrid::UpdateColorMap()
{
	const auto mapFactions = MapModel->GetMapFactionList();
	if (mapFactions.Num() != FactionColors.Num())
	{
		// Prepare
		FactionColors.Empty(mapFactions.Num());
		// This will work as long Id grows... in worst case everything will be one color...
		for (const auto& faction : mapFactions)
		{
			FactionColors.Add(faction.Id, Colors[faction.Id % Colors.Num()]);
		}
	}
}

void AUDSquareGrid::Update(TObjectPtr<UUDMapState> state)
{
	for (TObjectPtr<UUDTileState> dataTile : state->Tiles)
	{
		if (dataTile->Type == GridMap[dataTile->Position]->GetTileType())
		{
			// If we are not switching tile, then we need to update it!
			if (dataTile->OwnerUniqueId != GridMap[dataTile->Position]->GetTileOwner())
			{
				GridMap[dataTile->Position]->SetTileOwner(dataTile->OwnerUniqueId, FactionColors[dataTile->OwnerUniqueId]);
			}
			continue;
		}
		DeleteTile(dataTile);
		// Spawn new tile.
		CreateTile(dataTile);
		// Invoke update.
		GridMap[dataTile->Position]->OnUpdate();
	}
}

void AUDSquareGrid::Create(TObjectPtr<UUDMapState> state)
{
	for (TObjectPtr<UUDTileState> dataTile : state->Tiles)
	{
		// Spawn new tile.
		CreateTile(dataTile);
		// Invoke update.
		GridMap[dataTile->Position]->OnUpdate();
	}
}

void AUDSquareGrid::OnTileSelected(TObjectPtr<AUDSquareTile> newSelectedtile)
{
	// Just in case, something goes wrong...
	if (!newSelectedtile)
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Selected[%d][%d]."), newSelectedtile->GetTilePosition().X, newSelectedtile->GetTilePosition().Y);

	// If old is valid, we need to call deselect event.
	if (LastSelectedTile.IsValid())
	{
		LastSelectedTile->OnDeselected();
	}

	// We can safely swap to new tile.
	LastSelectedTile = newSelectedtile;
	newSelectedtile->OnSelected();
}

void AUDSquareGrid::DeleteTile(TObjectPtr<UUDTileState> dataTile)
{
	GridMap[dataTile->Position]->Destroy();
	GridMap.Remove(dataTile->Position);
}

void AUDSquareGrid::CreateTile(TObjectPtr<UUDTileState> dataTile)
{
	FVector worldPosition = CalculateTileWorldPosition(dataTile->Position);
	TSubclassOf<AUDSquareTile> tileClass = RetrieveTileType(dataTile);
	TObjectPtr<AUDSquareTile> newWorldTile = GetWorld()->SpawnActor<AUDSquareTile>(tileClass, worldPosition, FRotator::ZeroRotator);
	newWorldTile->SetTilePosition(dataTile->Position);
	newWorldTile->SetTileType(dataTile->Type);
	newWorldTile->SetTileOwner(dataTile->OwnerUniqueId, FactionColors[dataTile->OwnerUniqueId]);
	newWorldTile->Selected.BindUObject(this, &AUDSquareGrid::OnTileSelected);
	GridMap.Add(dataTile->Position, newWorldTile);
}

FVector AUDSquareGrid::CalculateTileWorldPosition(FIntPoint tilePosition)
{
	float x = tilePosition.X * TileHorizontalOffset;
	float y = tilePosition.Y * TileVerticalOffset;
	float z = 0;
	return FVector(x, y, z);
}

TSubclassOf<AUDSquareTile> AUDSquareGrid::RetrieveTileType(TObjectPtr<UUDTileState> tileClass)
{
	if (TypeIdTileTypeMap.Contains(tileClass->Type))
	{
		// Returns value in TileTypes based on index mapping of TypeMapping.
		return TypeIdTileTypeMap[tileClass->Type].TileType;
	}
	// Fallback...
	return FallbackTileType;
}