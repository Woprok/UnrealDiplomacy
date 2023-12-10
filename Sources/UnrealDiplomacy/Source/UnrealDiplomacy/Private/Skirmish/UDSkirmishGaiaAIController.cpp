// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionResourceDeficit.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionResourceYield.h"
#include "Core/Simulation/Actions/UDGaiaActionTileResourceYield.h"
#include "Core/Simulation/Actions/UDGaiaActionTileUpkeep.h"
#include "Core/Simulation/Actions/UDSystemActionRegentChange.h"

void AUDSkirmishGaiaAIController::ProcessIntermezzoPlay()
{
	MakeAction(UUDGaiaActionFactionResourceYield::ActionTypeId);
	MakeAction(UUDGaiaActionTileResourceYield::ActionTypeId);
	MakeAction(UUDGaiaActionTileUpkeep::ActionTypeId);
	MakeAction(UUDGaiaActionFactionResourceDeficit::ActionTypeId);

	// Finish this by executing regent change and ending the intermezzo.
	MakeAction(UUDSystemActionRegentChange::ActionTypeId);
}