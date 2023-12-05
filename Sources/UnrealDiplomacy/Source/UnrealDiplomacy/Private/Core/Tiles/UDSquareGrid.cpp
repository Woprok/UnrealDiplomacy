// Copyright Miroslav Valach

#include "Core/Tiles/UDSquareGrid.h"
#include "Core/Tiles/UDSquareTile.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishPlayerController.h"

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
	pc->OnSynchronizationFinishedEvent.AddUniqueDynamic(this, &AUDSquareGrid::OnSynchronized);
	pc->OnWorldSimulationUpdatedEvent.AddUniqueDynamic(this, &AUDSquareGrid::OnUpdate);
}

void AUDSquareGrid::OnUpdate(const FUDActionData& action)
{
	OnSynchronized();
}

void AUDSquareGrid::OnSynchronized()
{
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

void AUDSquareGrid::Update(TObjectPtr<UUDMapState> state)
{
	for (TObjectPtr<UUDTileState> dataTile : state->Tiles)
	{
		if (dataTile->Type == GridMap[dataTile->Position]->GetTileType())
		{
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

void AUDSquareGrid::OnTileSelected(TObjectPtr<AUDSquareTile> tile)
{
	UE_LOG(LogTemp, Log, TEXT("AUDSquareGrid: Selected[%d][%d]."), tile->GetTilePosition().X, tile->GetTilePosition().Y);
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