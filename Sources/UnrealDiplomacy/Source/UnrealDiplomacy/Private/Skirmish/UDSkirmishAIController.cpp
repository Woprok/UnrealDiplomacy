// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishAIController.h"

void AUDSkirmishAIController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDSkirmishAIController::GetControllerUniqueId()
{
	return UniqueControllerId;
}