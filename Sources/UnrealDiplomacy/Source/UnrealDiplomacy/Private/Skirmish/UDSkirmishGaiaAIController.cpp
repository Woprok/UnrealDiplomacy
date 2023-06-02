// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDGaiaActionResourcesAllAdd.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"

AUDSkirmishGaiaAIController::AUDSkirmishGaiaAIController()
{
	Type = EUDControllerType::Server;
}

void AUDSkirmishGaiaAIController::ProcessInTurnPlay()
{
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDGaiaActionResourcesAllAdd::ActionTypeId));

	// Finish this by executing end turn action, thus giving up control.
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDSystemActionTurnEnd::ActionTypeId));
}