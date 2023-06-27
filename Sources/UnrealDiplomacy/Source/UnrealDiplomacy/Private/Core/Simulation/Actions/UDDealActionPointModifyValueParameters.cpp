// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionPointModifyValueParameters.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionPointModifyValueParameters::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataPointParameters data(action.ValueParameters);
	bool hasChange = world->Deals[data.DealId]->Points[data.PointId]->ValueParameters != data.Parameters;
	return UUDDealActionPointModify::CanExecute(action, world) && hasChange;
}

void UUDDealActionPointModifyValueParameters::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Removes target from list of invokers.
	FUDDealDataPointParameters data(action.ValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->ValueParameters = data.Parameters;
}

void UUDDealActionPointModifyValueParameters::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Add target to list of invokers.
	FUDDealDataPointParameters data(action.ValueParameters);
	FUDDealDataPointParameters oldData(action.BackupValueParameters);
	world->Deals[data.DealId]->Points[data.PointId]->ValueParameters = oldData.Parameters;
}

void UUDDealActionPointModifyValueParameters::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDealDataPointParameters data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	for (auto param : world->Deals[data.DealId]->Points[data.PointId]->ValueParameters)
	{
		action.BackupValueParameters.Add(param);
	}
}

FUDActionPresentation UUDDealActionPointModifyValueParameters::GetPresentation() const
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