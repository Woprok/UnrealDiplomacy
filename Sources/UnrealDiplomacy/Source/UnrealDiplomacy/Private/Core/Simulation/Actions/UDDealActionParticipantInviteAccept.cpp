// Copyright Miroslav Valach
// TODO DELETE

#include "Core/Simulation/Actions/UDDealActionParticipantInviteAccept.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

//bool UUDDealActionParticipantInviteAccept::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
//{
//	FUDDealDataTarget data(action.ValueParameters);
//	bool isNotMember = !world->Deals[data.DealId]->Participants.Contains(data.TargetId);
//	bool isQueued = IsPendingTargetRequest(action, data.TargetId, world);
//	return IUDActionInterface::CanExecute(action, world) && isNotMember && isQueued;
//}
//
//void UUDDealActionParticipantInviteAccept::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Execute(action, world);
//	// Execute change based on data contained in confirm.
//	FUDDealDataTarget data(action.ValueParameters);
//	world->Deals[data.DealId]->Participants.Add(data.TargetId);
//	// Remove request from queue.
//	RemovePendingTargetRequest(action, data.TargetId, world);
//}
//
//void UUDDealActionParticipantInviteAccept::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
//{
//	IUDActionInterface::Revert(action, world);
//	// Confirmed request is returned to queue, but it has to be changed first.
//	FUDDealDataTarget data(action.ValueParameters);
//	FUDActionData originalActionData = FUDActionData::AsPredecessorOf(action, UUDDealActionParticipantInvite::ActionTypeId);
//	AddPendingTargetRequest(originalActionData, data.TargetId, world);
//	// Revert change based on data that were used for confirmation.
//	world->Deals[data.DealId]->Participants.Remove(data.TargetId);
//}