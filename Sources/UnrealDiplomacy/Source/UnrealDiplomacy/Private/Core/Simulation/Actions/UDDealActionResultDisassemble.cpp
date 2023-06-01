// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultDisassemble.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionResultDisassemble::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealData data(action.ValueParameters);
	bool isModerator = world->Deals[data.DealId]->OwnerUniqueId == action.InvokerFactionId;
	bool isStateBefore = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	return IUDActionInterface::CanExecute(action, world) && isModerator && isStateBefore;
}
void UUDDealActionResultDisassemble::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move to disassembled.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Disassembled;
}
void UUDDealActionResultDisassemble::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Move back to opened.
	FUDDealData data(action.ValueParameters);
	world->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}