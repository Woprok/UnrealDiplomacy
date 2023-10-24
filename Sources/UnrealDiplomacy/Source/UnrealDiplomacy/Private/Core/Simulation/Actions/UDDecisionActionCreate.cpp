// Copyright Miroslav Valach
#include "Core/Simulation/Actions/UDDecisionActionCreate.h"
#include "Core/Simulation/Actions/UDDecisionActionConfirm.h"
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
	EUDDecisionType decisionType = UUDDecisionAction::IntegerToDecisionType(data.Type);
	FUDActionData confirm = FUDActionData::FromValues(data.Parameters);
	FUDDecision newDecision;
	
	// This handles the demand consequence case 
	if (UUDDecisionAction::IntegerToDecisionType(data.Type) == EUDDecisionType::Demand)
	{
		FUDActionData decline(world->Factions[action.InvokerFactionId]->DecisionDemandPolicy, action.InvokerFactionId, { data.TargetId });
		newDecision = FUDDecision(decisionType, confirm, decline);
	}
	else
	{
		newDecision = FUDDecision(decisionType, confirm);
	}

	AddPendingTargetDecision(data.TargetId, action.UniqueId, newDecision, world);
	
}

void UUDDecisionActionCreate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDDecisionDataTargetTypeParameters data(action.ValueParameters);
	FUDActionData decisionAction = FUDActionData::FromValues(data.Parameters);
	RemovePendingTargetDecision(data.TargetId, action.UniqueId, world);
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

TArray<FUDActionData> UUDDecisionActionCreate::GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const
{
	TArray<FUDActionData> directExecution = { };
	FUDDecisionDataTargetTypeParameters data(parentAction.ValueParameters);
	// If enabled follows execution of the direct decisions. Thus this will call confirm on these decisions immediately as consequence.
	if (UUDGlobalGameConfig::IsDirectExecutionEnabled && 
		UUDDecisionAction::IntegerToDecisionType(data.Type) == EUDDecisionType::Gift)
	{
		FUDActionData decisionAction = FUDActionData(UUDDecisionActionConfirm::ActionTypeId, data.TargetId, { parentAction.UniqueId });
		directExecution.Add(decisionAction);
	}

	return directExecution;
}
