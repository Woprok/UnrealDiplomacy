// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDGaiaActionResourceYieldFaction.h"
#include "Core/Simulation/Actions/UDGaiaActionResourceYieldTile.h"
#include "Core/Simulation/Actions/UDSystemActionRegentChange.h"

void AUDSkirmishGaiaAIController::ProcessIntermezzoPlay()
{
	MakeAction(UUDGaiaActionResourceYieldFaction::ActionTypeId);
	MakeAction(UUDGaiaActionResourceYieldTile::ActionTypeId);

	// Finish this by executing regent change and ending the intermezzo.
	MakeAction(UUDSystemActionRegentChange::ActionTypeId);
}