// Copyright Miroslav Valach

#include "Core/UDPlayerController.h"

AUDPlayerController::AUDPlayerController()
{
	bEnableClickEvents = true;
}

void AUDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Overrides default input mode to be Game & UI allowing player to use cursor & navigate the world.
	FInputModeGameAndUI inputMode = FInputModeGameAndUI();
	inputMode.SetHideCursorDuringCapture(false);
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	AUDPlayerController::SetInputMode(inputMode);
	SetShowMouseCursor(true);
}