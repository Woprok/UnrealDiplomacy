// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDChatViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDInviteItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/Actions/UDDealActionResultClose.h"
#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"
#include "Core/Simulation/Actions/UDDealActionReady.h"
#include "Core/Simulation/Actions/UDDealActionReadyRevert.h"
#include "Core/Simulation/Actions/UDDealActionVoteYes.h"
#include "Core/Simulation/Actions/UDDealActionVoteNo.h"
#include "Core/Simulation/Actions/UDDealActionContractCreate.h"
#include "Core/Simulation/Actions/UDDealActionContractExecute.h"
#include "Core/UDGlobalData.h"

#define LOCTEXT_NAMESPACE "DealGeneralTab"

void UUDDealGeneralTabViewModel::Initialize()
{
	ParticipantItemViewModelType = UUDParticipantItemViewModel::StaticClass();
	InviteItemViewModelType = UUDInviteItemViewModel::StaticClass();
	ChatViewModelType = UUDChatViewModel::StaticClass();

	SetIsModeratorValue(false);
	FText stateTitle = FText(LOCTEXT("DealGeneralTab", "Deal Status"));
	SetStateTitleText(stateTitle);
	FText participant = FText(LOCTEXT("DealGeneralTab", "Participants"));
	SetParticipantText(participant);
	FText invite = FText(LOCTEXT("DealGeneralTab", "Non-Participants"));
	SetInviteableText(invite);
	
	FText ready = FText(LOCTEXT("DealGeneralTab", "Ready"));
	SetReadyText(ready);
	FText vote = FText(LOCTEXT("DealGeneralTab", "Vote"));
	SetVoteText(vote);
	FText leave = FText(LOCTEXT("DealGeneralTab", "Leave"));
	SetLeaveText(leave);
	FText cancel = FText(LOCTEXT("DealGeneralTab", "Cancel"));
	SetCancelText(cancel);
	
	FText createContract = FText(LOCTEXT("DealGeneralTab", "Create Contract"));
	SetCreateContractText(createContract);
	FText executeContract = FText(LOCTEXT("DealGeneralTab", "Execute Contract"));
	SetExecuteContractText(executeContract);

	Update();
}

void UUDDealGeneralTabViewModel::Reload()
{
	Update();
}

void UUDDealGeneralTabViewModel::Update()
{
	if (!Model->IsOverseeingStatePresent())
		return;
	if (!Model->IsGamePlayed())
		return;
	if (Content.DealId == UUDGlobalData::InvalidDealId)
		return;
	// Following updates require model.

	FUDDealInfo dealInfo = Model->GetDealInfo(Content.DealId);
	SetIsModeratorValue(dealInfo.IsModerator);
	SetStateText(FText::FromString(dealInfo.State));
	FText readyCount = FText::Format(LOCTEXT("DealGeneralTab", "Ready {0}/{1}"), dealInfo.ReadyCount, dealInfo.ParticipantCount);
	SetReadyCountText(readyCount);
	FText voteCount = FText::Format(LOCTEXT("DealGeneralTab", "Vote Yes {0}/{1}"), dealInfo.VoteCount, dealInfo.ParticipantCount);
	SetVoteCountText(voteCount);

	UpdateChatInstance();
	UpdateParticipantItemList();
	UpdateInviteItemList();
}

#undef LOCTEXT_NAMESPACE

void UUDDealGeneralTabViewModel::ChangeReady()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: ChangeReady."));
	FUDDealInfo fullContent = Model->GetDealInfo(Content.DealId);
	if (fullContent.LocalReady)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionReadyRevert::ActionTypeId, { Content.DealId }));
	}
	else
	{
		Model->RequestAction(Model->GetAction(UUDDealActionReady::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::ChangeVote()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: ChangeVote."));
	FUDDealInfo fullContent = Model->GetDealInfo(Content.DealId);
	if (fullContent.LocalVote)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionVoteNo::ActionTypeId, { Content.DealId }));
	}
	else
	{
		Model->RequestAction(Model->GetAction(UUDDealActionVoteYes::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::Leave()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: Leave."));
	if (!IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionParticipantLeave::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::Cancel()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: Cancel."));
	if (IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionResultClose::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::CreateContract()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: CreateContract."));
	if (IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionContractCreate::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::ExecuteContract()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: ExecuteContract."));
	if (IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionContractExecute::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::InvalidateContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: InvalidateContent."));
	Content = content;
}

void UUDDealGeneralTabViewModel::SetContent(FUDDealMinimalInfo content)
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: SetContent."));
	Content = content;
}

void UUDDealGeneralTabViewModel::UpdateChatInstance()
{
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> viewModel = hud->GetViewModelCollection(ChatViewModelInstanceName, ChatViewModelType);
	ChatViewModelInstance = Cast<UUDChatViewModel>(viewModel);
	SetDealChatContent(FUDViewModelContent(ChatViewModelInstance));
	ChatViewModelInstance->SetContent(Content);
	ChatViewModelInstance->FullUpdate();
}

void UUDDealGeneralTabViewModel::UpdateParticipantItemList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: UpdateParticipantItemList."));
	// Retrieve factions
	TArray<FUDDealFactionInfo> messages = Model->GetDealParticipantList(Content.DealId);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ParticipantItemViewModelCollectionName,
		ParticipantItemViewModelType, messages.Num());
	// Get rid of all models
	ParticipantItemViewModelCollection.Empty();
	for (int32 i = 0; i < messages.Num(); i++)
	{
		TObjectPtr<UUDParticipantItemViewModel> newViewModel = Cast<UUDParticipantItemViewModel>(viewModels[i]);
		newViewModel->SetContent(messages[i]);
		newViewModel->FullUpdate();
		ParticipantItemViewModelCollection.Add(newViewModel);
	}

	SetParticipantItemList(FUDViewModelList(viewModels));
}

void UUDDealGeneralTabViewModel::UpdateInviteItemList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: UpdateInviteItemList."));
	// Retrieve factions
	TArray<FUDDealFactionInfo> messages = Model->GetDealInviteList(Content.DealId);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(InviteItemViewModelCollectionName,
		InviteItemViewModelType, messages.Num());
	// Get rid of all models
	InviteItemViewModelCollection.Empty();
	for (int32 i = 0; i < messages.Num(); i++)
	{
		TObjectPtr<UUDInviteItemViewModel> newViewModel = Cast<UUDInviteItemViewModel>(viewModels[i]);
		newViewModel->SetContent(messages[i]);
		newViewModel->FullUpdate();
		InviteItemViewModelCollection.Add(newViewModel);
	}

	SetInviteItemList(FUDViewModelList(viewModels));
}

void UUDDealGeneralTabViewModel::SetIsModeratorValue(bool newIsModeratorValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsModeratorValue, newIsModeratorValue);
}

bool UUDDealGeneralTabViewModel::GetIsModeratorValue() const
{
	return IsModeratorValue;
}

void UUDDealGeneralTabViewModel::SetStateTitleText(FText newStateTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StateTitleText, newStateTitleText);
}

FText UUDDealGeneralTabViewModel::GetStateTitleText() const
{
	return StateTitleText;
}

void UUDDealGeneralTabViewModel::SetStateText(FText newStateText)
{
	UE_MVVM_SET_PROPERTY_VALUE(StateText, newStateText);
}

FText UUDDealGeneralTabViewModel::GetStateText() const
{
	return StateText;
}

void UUDDealGeneralTabViewModel::SetParticipantText(FText newParticipantText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ParticipantText, newParticipantText);
}

FText UUDDealGeneralTabViewModel::GetParticipantText() const
{
	return ParticipantText;
}

void UUDDealGeneralTabViewModel::SetReadyCountText(FText newReadyCountText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ReadyCountText, newReadyCountText);
}

FText UUDDealGeneralTabViewModel::GetReadyCountText() const
{
	return ReadyCountText;
}

void UUDDealGeneralTabViewModel::SetVoteCountText(FText newVoteCountText)
{
	UE_MVVM_SET_PROPERTY_VALUE(VoteCountText, newVoteCountText);
}

FText UUDDealGeneralTabViewModel::GetVoteCountText() const
{
	return VoteCountText;
}

void UUDDealGeneralTabViewModel::SetInviteableText(FText newInviteableText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InviteableText, newInviteableText);
}

FText UUDDealGeneralTabViewModel::GetInviteableText() const
{
	return InviteableText;
}

void UUDDealGeneralTabViewModel::SetReadyText(FText newReadyText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ReadyText, newReadyText);
}

FText UUDDealGeneralTabViewModel::GetReadyText() const
{
	return ReadyText;
}

void UUDDealGeneralTabViewModel::SetVoteText(FText newVoteText)
{
	UE_MVVM_SET_PROPERTY_VALUE(VoteText, newVoteText);
}

FText UUDDealGeneralTabViewModel::GetVoteText() const
{
	return VoteText;
}

void UUDDealGeneralTabViewModel::SetLeaveText(FText newLeaveText)
{
	UE_MVVM_SET_PROPERTY_VALUE(LeaveText, newLeaveText);
}

FText UUDDealGeneralTabViewModel::GetLeaveText() const
{
	return LeaveText;
}

void UUDDealGeneralTabViewModel::SetCancelText(FText newCancelText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CancelText, newCancelText);
}

FText UUDDealGeneralTabViewModel::GetCancelText() const
{
	return CancelText;
}

void UUDDealGeneralTabViewModel::SetExecuteContractText(FText newExecuteContractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ExecuteContractText, newExecuteContractText);
}

FText UUDDealGeneralTabViewModel::GetExecuteContractText() const
{
	return ExecuteContractText;
}

void UUDDealGeneralTabViewModel::SetCreateContractText(FText newCreateContractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreateContractText, newCreateContractText);
}

FText UUDDealGeneralTabViewModel::GetCreateContractText() const
{
	return CreateContractText;
}

void UUDDealGeneralTabViewModel::SetParticipantItemList(FUDViewModelList newParticipantItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ParticipantItemList, newParticipantItemList);
}

FUDViewModelList UUDDealGeneralTabViewModel::GetParticipantItemList() const
{
	return ParticipantItemList;
}

void UUDDealGeneralTabViewModel::SetInviteItemList(FUDViewModelList newInviteItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(InviteItemList, newInviteItemList);
}

FUDViewModelList UUDDealGeneralTabViewModel::GetInviteItemList() const
{
	return InviteItemList;
}

void UUDDealGeneralTabViewModel::SetDealChatContent(FUDViewModelContent newDealChatContent)
{
	UE_MVVM_SET_PROPERTY_VALUE(DealChatContent, newDealChatContent);
}

FUDViewModelContent UUDDealGeneralTabViewModel::GetDealChatContent() const
{
	return DealChatContent;
}