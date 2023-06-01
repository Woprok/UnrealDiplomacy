// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDGaiaAction::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isGaia = action.InvokerFactionId == UUDGlobalData::GaiaId;
	return IUDActionInterface::CanExecute(action, world) && isGaia;
}