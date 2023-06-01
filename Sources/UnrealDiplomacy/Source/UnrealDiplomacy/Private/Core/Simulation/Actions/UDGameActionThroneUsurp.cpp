// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionThroneUsurp::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isThroneEmpty = world->ImperialThrone.Ruler == UUDGlobalData::GaiaId;
	return IUDActionInterface::CanExecute(action, world) && isThroneEmpty;
}

void UUDGameActionThroneUsurp::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Takeover the empty throne.
	world->ImperialThrone.Ruler = action.InvokerFactionId;
}

void UUDGameActionThroneUsurp::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Rollback to the empty throne.
	world->ImperialThrone.Ruler = UUDGlobalData::GaiaId;
}