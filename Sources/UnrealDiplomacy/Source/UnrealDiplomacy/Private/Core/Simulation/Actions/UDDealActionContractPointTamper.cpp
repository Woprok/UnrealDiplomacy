// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionContractPointTamper::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataContractPointParameters data(action.ValueParameters);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState == EUDDealSimulationState::ResolutionOfPoints;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult == EUDDealSimulationResult::Opened;
	bool isNotResolved = world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult == EUDDealActionResult::Unresolved;
	return IUDActionInterface::CanExecute(action, world) && isNotResolved && isStateOpen && isResultOpen;
}

void UUDDealActionContractPointTamper::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Tampers with the contract point.
	FUDDealDataContractPointParameters data(action.ValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult = EUDDealActionResult::Changed;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].Action.TextParameter = action.TextParameter;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].Action.ValueParameters = data.Parameters;
}

void UUDDealActionContractPointTamper::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Revert to undecided state of contract point.
	FUDDealDataContractPointParameters data(action.ValueParameters);
	FUDDealDataContractPointParameters oldData(action.BackupValueParameters);
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].SelectedResult = EUDDealActionResult::Unresolved;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].Action.TextParameter = action.BackupTextParameter;
	world->Deals[data.DealId]->DealActionList[data.ContractPointId].Action.ValueParameters = oldData.Parameters;
}

void UUDDealActionContractPointTamper::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealDataContractPointParameters data(action.ValueParameters);
	action.BackupTextParameter = world->Deals[data.DealId]->DealActionList[data.ContractPointId].Action.TextParameter;
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(data.DealId);
	action.BackupValueParameters.Add(data.ContractPointId);
	auto oldValueParams = world->Deals[data.DealId]->DealActionList[data.ContractPointId].Action.ValueParameters;
	action.BackupValueParameters.Append(oldValueParams);
}

FUDActionPresentation UUDDealActionContractPointTamper::GetPresentation() const
{
	FUDActionPresentation presentation = Super::GetPresentation();
	presentation.ActionId = GetId();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VERIFY_PARAMETER_MINIMUM,
		}
	);

	return presentation;
}