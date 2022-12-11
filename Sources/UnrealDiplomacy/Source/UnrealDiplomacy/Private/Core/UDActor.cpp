// Copyright Miroslav Valach

#include "Core/UDActor.h"

AUDActor::AUDActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AUDActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUDActor::BeginPlay()
{
	Super::BeginPlay();
}