// Copyright Miroslav Valach


#include "Core/UDActor.h"

// Sets default values
AUDActor::AUDActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUDActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUDActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

