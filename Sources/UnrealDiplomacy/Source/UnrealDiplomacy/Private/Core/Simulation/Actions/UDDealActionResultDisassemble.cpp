// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionResultDisassemble.h"

bool UUDDealActionResultDisassemble::CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	bool result = IUDActionInterface::CanExecute(actionData, targetWorldState);
	if (result)
	{
		FUDDealData data = UUDDealActionResultDisassemble::ConvertData(actionData);
		bool isModerator = targetWorldState->Deals[data.DealId]->OwnerUniqueId == actionData.InvokerPlayerId;
		bool isStateBefore = targetWorldState->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
		result = result && isModerator && isStateBefore;
	}
	return result;
}
void UUDDealActionResultDisassemble::Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDDealActionResultDisassemble::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Disassembled;
}
void UUDDealActionResultDisassemble::Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState)
{
	FUDDealData data = UUDDealActionResultDisassemble::ConvertData(actionData);
	targetWorldState->Deals[data.DealId]->DealSimulationResult = EUDDealSimulationResult::Opened;
}