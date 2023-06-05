// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDSettingAction.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDSettingAction::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	// All settings action are available only during the game creation.
	bool isPreparing = world->WorldSimulationState == EUDWorldSimulationState::Preparing;
	return IUDActionInterface::CanExecute(action, world) && isPreparing;
}