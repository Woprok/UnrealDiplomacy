// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSystemActionIntermezzoStart.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDSystemActionIntermezzoStart::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isNotIntermezzo = !world->TurnData.IsIntermezzo;
	bool isGaia = action.InvokerFactionId == UUDGlobalData::GaiaFactionId;
	return IUDActionInterface::CanExecute(action, world) && isNotIntermezzo && isGaia;
}

void UUDSystemActionIntermezzoStart::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Revert intermezzo to active.
	world->TurnData.IsIntermezzo = true;
}

void UUDSystemActionIntermezzoStart::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Deactivate intermezzo.
	world->TurnData.IsIntermezzo = false;
}