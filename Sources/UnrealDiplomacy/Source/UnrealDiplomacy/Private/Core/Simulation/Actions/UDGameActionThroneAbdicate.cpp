// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGameActionThroneAbdicate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isRuler = world->ImperialThrone.Ruler == action.InvokerPlayerId;
	return IUDActionInterface::CanExecute(action, world) && isRuler;
}

void UUDGameActionThroneAbdicate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Player is removed from the throne..
	world->ImperialThrone.Ruler = UUDGlobalData::GaiaId;
}

void UUDGameActionThroneAbdicate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Player is returned to throne.
	world->ImperialThrone.Ruler = action.InvokerPlayerId;
}