// Copyright Miroslav Valach


#include "Core/Map/AUDHexTile.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
AUDHexTile::AUDHexTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
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

