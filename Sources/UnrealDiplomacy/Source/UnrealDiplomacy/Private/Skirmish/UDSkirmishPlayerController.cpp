// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishPlayerController.h"

void AUDSkirmishPlayerController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDSkirmishPlayerController::GetControllerUniqueId()
{
	return UniqueControllerId;
}
