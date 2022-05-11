// Copyright Miroslav Valach


#include "Core/Map/AUDHexTile.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>

#include "Net/UnrealNetwork.h"

// Sets default values
AUDHexTile::AUDHexTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AUDHexTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUDHexTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUDHexTile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUDHexTile, GridIndices);
	DOREPLIFETIME(AUDHexTile, DataIndices);
	DOREPLIFETIME(AUDHexTile, TileType);
	DOREPLIFETIME(AUDHexTile, TileMesh);
}

//AUDHexTile::AUDHexTile(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
//{
//}