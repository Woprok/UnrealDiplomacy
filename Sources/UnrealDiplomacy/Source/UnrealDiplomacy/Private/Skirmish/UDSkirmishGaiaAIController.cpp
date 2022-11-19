// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishGaiaAIController.h"

void AUDSkirmishGaiaAIController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDSkirmishGaiaAIController::GetControllerUniqueId()
{
	return UniqueControllerId;
}