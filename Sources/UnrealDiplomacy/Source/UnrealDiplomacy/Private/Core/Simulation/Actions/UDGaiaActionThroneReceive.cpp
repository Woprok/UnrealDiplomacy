// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaActionThroneReceive.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGaiaActionThroneReceive::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isThroneEmpty = world->ImperialThrone.Ruler == UUDGlobalData::GaiaFactionId;
	return UUDGaiaAction::CanExecute(action, world) && isThroneEmpty;
}

void UUDGaiaActionThroneReceive::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Takeover the empty throne.
	FUDGaiaDataTarget data(action.ValueParameters);
	world->ImperialThrone.Ruler = data.TargetId;
}

void UUDGaiaActionThroneReceive::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Rollback to the empty throne.
	world->ImperialThrone.Ruler = UUDGlobalData::GaiaFactionId;
}