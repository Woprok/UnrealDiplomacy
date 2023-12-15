// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInviteConsequence.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionParticipantInviteConsequence::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataTarget data(action.ValueParameters);
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	bool isNotMember = !world->Deals[data.DealId]->Participants.Contains(data.TargetId);
	bool isNotBlocked = !world->Deals[data.DealId]->BlockedParticipants.Contains(data.TargetId);
	return IUDActionInterface::CanExecute(action, world) && isNotMember && isNotBlocked && isResultOpen;
}

void UUDDealActionParticipantInviteConsequence::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Block player future participation.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->BlockedParticipants.Add(data.TargetId);
}

void UUDDealActionParticipantInviteConsequence::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Enable blocked player participation.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->BlockedParticipants.Remove(data.TargetId);
}

#define LOCTEXT_NAMESPACE "ParticipantInviteConsequence"
FUDActionPresentation UUDDealActionParticipantInviteConsequence::GetPresentation() const
{
	if (GetId() != UUDDealActionParticipantInviteConsequence::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ParticipantInviteConsequence", "Deal Decline")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_PARAMETER_DEAL,
			UD_ACTION_TAG_PARAMETER_FACTION,
		}
	);

	//presentation.RejectActionId = UUDDealActionParticipantInviteReject::ActionTypeId;
	presentation.MessageContentFormat = FText(LOCTEXT("ParticipantInviteConsequence",
		"Your faction [{TARGET}] declines invite to participate in [{INVOKER}]'s deal: [{DEAL}] permanently."
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("ParticipantInviteConsequence",
		"Faction [{INVOKER}] will invite [{TARGET}] to [{DEAL}].\nReport this as bug please!"
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE