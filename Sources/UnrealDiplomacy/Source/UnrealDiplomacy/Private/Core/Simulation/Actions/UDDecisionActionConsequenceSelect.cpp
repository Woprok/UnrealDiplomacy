// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDecisionActionConsequenceSelect.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDecisionActionConsequenceSelect::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	return IUDActionInterface::CanExecute(action, world);
}

void UUDDecisionActionConsequenceSelect::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Change consequence policy to new.
	FUDDecisionDataAction data(action.ValueParameters);
	world->Factions[action.InvokerFactionId]->DecisionDemandPolicy = data.ActionId;
}

void UUDDecisionActionConsequenceSelect::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Change consequence policy to old.
	FUDDecisionDataAction data(action.BackupValueParameters);
	world->Factions[action.InvokerFactionId]->DecisionDemandPolicy = data.ActionId;
}

void UUDDecisionActionConsequenceSelect::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old action is backuped for future revert use.
	FUDDecisionDataAction data(action.ValueParameters);
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(world->Factions[action.InvokerFactionId]->DecisionDemandPolicy);
}