// Copyright Miroslav Valach

#include "Core/UDPlayerController.h"

void AUDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Overrides default input mode to be Game & UI allowing player to use cursor & navigate the world.
	AUDPlayerController::SetInputMode(FInputModeGameAndUI());
}