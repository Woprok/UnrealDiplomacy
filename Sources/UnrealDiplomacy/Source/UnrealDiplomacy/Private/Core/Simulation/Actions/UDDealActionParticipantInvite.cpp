// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

bool UUDDealActionParticipantInvite::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	FUDDealDataTarget data(action.ValueParameters);
	bool isNotMember = !world->Deals[data.DealId]->Participants.Contains(data.TargetId);
	bool isStateOpen = world->Deals[data.DealId]->DealSimulationState <= EUDDealSimulationState::Assembling;
	bool isResultOpen = world->Deals[data.DealId]->DealSimulationResult <= EUDDealSimulationResult::Opened;
	bool isNotLeaver = !world->Deals[data.DealId]->BlockedParticipants.Contains(data.TargetId);

	return IUDActionInterface::CanExecute(action, world) && isStateOpen && isResultOpen && isNotLeaver && isNotMember;
}

void UUDDealActionParticipantInvite::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);
	// Queue new confirmable request.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Participants.Add(data.TargetId);
}

void UUDDealActionParticipantInvite::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Remove request from queue.
	FUDDealDataTarget data(action.ValueParameters);
	world->Deals[data.DealId]->Participants.Remove(data.TargetId);
}

#define LOCTEXT_NAMESPACE "ParticipantInvite"
FUDActionPresentation UUDDealActionParticipantInvite::GetPresentation() const
{
	if (GetId() != UUDDealActionParticipantInvite::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ParticipantInvite", "Deal Invite")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_PARAMETER_DEAL,
			UD_ACTION_TAG_PARAMETER_FACTION,
		}
	);

	//presentation.RejectActionId = UUDDealActionParticipantInviteReject::ActionTypeId;
	presentation.MessageContentFormat = FText(LOCTEXT("ParticipantInvite",
		"Your faction [{TARGET}] has been invited to participate in [{INVOKER}]'s deal: [{DEAL}].\nHow do you answer ?"
	)).ToString();
	presentation.DealContentFormat = FText(LOCTEXT("ParticipantInvite",
		"Faction [{INVOKER}] will invite [{TARGET}] to [{DEAL}].\nReport this as bug please!"
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE