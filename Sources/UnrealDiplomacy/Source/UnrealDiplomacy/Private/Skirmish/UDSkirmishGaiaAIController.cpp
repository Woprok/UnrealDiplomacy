// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDGaiaActionResourcesAllAdd.h"
#include "Core/Simulation/Actions/UDSystemActionRegentChange.h"

void AUDSkirmishGaiaAIController::ProcessIntermezzoPlay()
{
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDGaiaActionResourcesAllAdd::ActionTypeId));

	// Finish this by executing regent change and ending the intermezzo.
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDSystemActionRegentChange::ActionTypeId));
}