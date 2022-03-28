// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AUDHexTile.generated.h"

enum class EHexTileType : uint8;
class UStaticMeshComponent;

UCLASS()
class UNREALDIPLOMACY_API AUDHexTile : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Tile")
	FIntPoint TileIndex;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	EHexTileType TileType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	TObjectPtr<UStaticMeshComponent> TileMesh;
	
public:
	// Sets default values for this actor's properties
	AUDHexTile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
