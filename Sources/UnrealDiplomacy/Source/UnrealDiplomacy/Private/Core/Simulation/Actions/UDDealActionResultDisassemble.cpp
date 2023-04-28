// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultDisassemble.h"

bool UUDDealActionResultDisassemble::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	bool result = IUDActionInterface::CanExecute(data, world);
	if (result)
	{
		FUDDealData data = UUDDealActionResultDisassemble::ConvertData(data);
		bool isModerator = world->Deals[action.DealId]->OwnerUniqueId == action.InvokerPlayerId;
		bool isStateBefore = world->Deals[action.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultDisassemble::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	FUDDealData data = UUDDealActionResultDisassemble::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Disassembled;
}
void UUDDealActionResultDisassemble::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	FUDDealData data = UUDDealActionResultDisassemble::ConvertData(data);
	world->Deals[action.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}