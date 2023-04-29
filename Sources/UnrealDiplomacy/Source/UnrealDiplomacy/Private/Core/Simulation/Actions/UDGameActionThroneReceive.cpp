// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneReceive.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionThroneReceive::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isGaia = action.InvokerPlayerId == UUDGlobalData::GaiaId;
	bool isThroneEmpty = world->ImperialThrone.Ruler == UUDGlobalData::GaiaId;
	return IUDActionInterface::CanExecute(action, world) && isGaia && isThroneEmpty;
}

void UUDGameActionThroneReceive::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Takeover the empty throne.
	FUDGameDataTarget data(action.ValueParameters);
	world->ImperialThrone.Ruler = data.TargetId;
}

void UUDGameActionThroneReceive::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Rollback to the empty throne.
	world->ImperialThrone.Ruler = UUDGlobalData::GaiaId;
}