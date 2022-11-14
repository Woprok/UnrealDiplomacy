// Copyright Miroslav Valach


#include "Core/UDPlayerController.h"

void AUDPlayerController::BeginPlay()
{
	// Forces default behaviour of input on all player controllers.
	// Basically we always interact with UI and Widgets so there is never a reason for it
	// to use any other mode.
	Super::BeginPlay();
	AUDPlayerController::SetInputMode(FInputModeGameAndUI());
}