// Copyright Miroslav Valach
// TODO consider adding continuation for executing direct actions such as gift.

#include "Core/Simulation/Actions/UDDecisionActionCreate.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDecisionActionCreate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	//bool isPlayer = action.InvokerFactionId != UUDGlobalData::GaiaFactionId;
	//bool isQueuedAgain = IsPendingInterchangeableTargetDecision(action, data.TargetId, world);
	// TODO prevent duplicates
	return IUDActionInterface::CanExecute(action, world);
}

void UUDDecisionActionCreate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDDecisionDataTargetTypeParameters data(action.ValueParameters);
	FUDActionData decisionAction = FUDActionData::FromValues(data.Parameters);
	AddPendingTargetDecision(decisionAction, data.TargetId, UUDDecisionAction::IntegerToDecisionType(data.Type), action.UniqueId, world);
}

void UUDDecisionActionCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDDecisionDataTargetTypeParameters data(action.ValueParameters);
	FUDActionData decisionAction = FUDActionData::FromValues(data.Parameters);
	RemovePendingTargetDecision(decisionAction, data.TargetId, action.UniqueId, world);
}

FUDActionPresentation UUDDecisionActionCreate::GetPresentation() const
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