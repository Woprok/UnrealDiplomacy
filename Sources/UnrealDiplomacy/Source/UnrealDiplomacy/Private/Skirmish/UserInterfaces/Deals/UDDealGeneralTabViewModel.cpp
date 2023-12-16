// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDChatViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDParticipantItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDInviteItemViewModel.h"
#include "Skirmish/UDSkirmishHUD.h"
#include "Skirmish/UDSkirmishPlayerController.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/Actions/UDDealActionClose.h"
#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"
#include "Core/Simulation/Actions/UDDealActionReady.h"
#include "Core/Simulation/Actions/UDDealActionReadyRevert.h"
#include "Core/Simulation/Actions/UDDealActionVoteYes.h"
#include "Core/Simulation/Actions/UDDealActionVoteNo.h"
#include "Core/Simulation/Actions/UDDealActionEndStateAssemble.h"
#include "Core/Simulation/Actions/UDDealActionEndStateDefine.h"
#include "Core/Simulation/Actions/UDDealActionEndStateVote.h"
#include "Core/Simulation/Actions/UDDealActionEndStateResolution.h"
#include "Core/Simulation/Actions/UDDealActionContractExecute.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "DealGeneralTab"

FText ConstructDealHelp()
{
	TStringBuilder<128> content;

	content.Append(FText(LOCTEXT("DealGeneralTab", "Deal is split into multiple phases:")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());

	content.Append(FText(LOCTEXT("DealGeneralTab", "1. Assemble Phase")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Factions can join by invite/leave/be kicked from the deal.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Regent can start discussions.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - The next phase requires 2 or more participants.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());

	content.Append(FText(LOCTEXT("DealGeneralTab", "2. Discussion Phase")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Factions can add/edit/ignore discussion points.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Refer to the 'Point Discussion' tab.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Regent can start a vote.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());

	content.Append(FText(LOCTEXT("DealGeneralTab", "3. Vote Phase")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Factions have a last chance to change their vote.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Regent can end the vote.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - The next phase requires a unanimous vote.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());

	content.Append(FText(LOCTEXT("DealGeneralTab", "4. Resolution Phase")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Factions can resolve each item (Accept/Deny/Change/Sabotage).")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Refer to the 'Resolution' tab.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Regent can finish the deal.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - The next phase requires all points to be resolved.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());

	content.Append(FText(LOCTEXT("DealGeneralTab", "5. Execution Phase")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Regent can execute the deal once before the game ends.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText(LOCTEXT("DealGeneralTab", " - Based on the resolution, all participants receive reputation.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());

	return FText::FromString(content.ToString());
}

void UUDDealGeneralTabViewModel::Setup()
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
	FText voteHelp = FText(LOCTEXT("DealGeneralTab", "Vote can be switched between Yes / No by repeated use, before vote phase is over."));
	SetVoteHelpText(voteHelp);
	FText leave = FText(LOCTEXT("DealGeneralTab", "Leave"));
	SetLeaveText(leave);
	FText cancel = FText(LOCTEXT("DealGeneralTab", "Cancel"));
	SetCancelText(cancel);

	FText assemble = FText(LOCTEXT("DealGeneralTab", "Start Discussion"));
	SetEndAssembleText(assemble);
	FText define = FText(LOCTEXT("DealGeneralTab", "Start Vote"));
	SetEndDefineText(define);
	FText votePhase = FText(LOCTEXT("DealGeneralTab", "End Vote"));
	SetEndVoteText(votePhase);
	FText resolution = FText(LOCTEXT("DealGeneralTab", "Finish Deal"));
	SetEndResolutionText(resolution);
	FText executeContract = FText(LOCTEXT("DealGeneralTab", "Execute Deal"));
	SetExecuteContractText(executeContract);

	FText helpTitle = FText(LOCTEXT("DealGeneralTab", "Deal Process Guide"));
	SetHelpTitleText(helpTitle);
	FText help = ConstructDealHelp();
	SetHelpText(help);

	SetCanEndAssembleValue(false);
	SetCanEndDefineValue(false);
	SetCanEndVoteValue(false);
	SetCanEndResolutionValue(false);
	SetCanExecuteContractValue(false);
	SetCanCloseDealValue(false);

	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TObjectPtr<UUDViewModel> chatViewModel = hud->GetViewModelCollection(ChatViewModelInstanceName, ChatViewModelType);
	ChatViewModelInstance = Cast<UUDChatViewModel>(chatViewModel);
	SetDealChatContent(FUDViewModelContent(ChatViewModelInstance));
}

void UUDDealGeneralTabViewModel::Refresh()
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
	FText voteCount = FText::Format(LOCTEXT("DealGeneralTab", "Voted {0}/{1}"), dealInfo.VoteCount, dealInfo.ParticipantCount);
	SetVoteCountText(voteCount);

	UpdateExecution();
	UpdateChatInstance();
	UpdateParticipantItemList();
	UpdateInviteItemList();
}

#undef LOCTEXT_NAMESPACE

void UUDDealGeneralTabViewModel::UpdateExecution()
{
	FUDDealPhaseInfo dealPhaseInfo = Model->GetDealPhaseInfo(Content.DealId);

	SetCanEndAssembleValue(dealPhaseInfo.CanEndAssemble);
	SetCanEndDefineValue(dealPhaseInfo.CanEndDefine);
	SetCanEndVoteValue(dealPhaseInfo.CanEndVote);
	SetCanEndResolutionValue(dealPhaseInfo.CanEndResolution);
	SetCanExecuteContractValue(dealPhaseInfo.CanEndExecute);
	SetCanCloseDealValue(dealPhaseInfo.CanClose);
}

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
		Model->RequestAction(Model->GetAction(UUDDealActionClose::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::EndAssemble()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: EndAssemble."));
	if (IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionEndStateAssemble::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::EndDefine()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: EndDefine."));
	if (IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionEndStateDefine::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::EndVote()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: EndVote."));
	if (IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionEndStateVote::ActionTypeId, { Content.DealId }));
	}
}

void UUDDealGeneralTabViewModel::EndResolution()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: EndResolution."));
	if (IsModeratorValue)
	{
		Model->RequestAction(Model->GetAction(UUDDealActionEndStateResolution::ActionTypeId, { Content.DealId }));
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
	ChatViewModelInstance->SetContent(Content);
	ChatViewModelInstance->Refresh();
}

void UUDDealGeneralTabViewModel::UpdateParticipantItemList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDDealGeneralTabViewModel: UpdateParticipantItemList."));
	// Retrieve factions
	TArray<FUDDealFactionInfo> messages = Model->GetDealParticipantList(Content.DealId);
	// Retrieve enough models
	TObjectPtr<AUDSkirmishHUD> hud = AUDSkirmishHUD::Get(GetWorld());
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(ParticipantItemViewModelCollectionName,
		ParticipantItemViewModelType, messages.Num());
	// Get rid of all models
	ParticipantItemViewModelCollection.Empty();
	for (int32 i = 0; i < messages.Num(); i++)
	{
		TObjectPtr<UUDParticipantItemViewModel> newViewModel = Cast<UUDParticipantItemViewModel>(viewModels[i]);
		ParticipantItemViewModelCollection.Add(newViewModel);
		newViewModel->SetContent(messages[i]);
		newViewModel->Refresh();
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
	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(InviteItemViewModelCollectionName,
		InviteItemViewModelType, messages.Num());
	// Get rid of all models
	InviteItemViewModelCollection.Empty();
	for (int32 i = 0; i < messages.Num(); i++)
	{
		TObjectPtr<UUDInviteItemViewModel> newViewModel = Cast<UUDInviteItemViewModel>(viewModels[i]);
		InviteItemViewModelCollection.Add(newViewModel);
		newViewModel->SetContent(messages[i]);
		newViewModel->Refresh();
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

void UUDDealGeneralTabViewModel::SetVoteHelpText(FText newVoteHelpText)
{
	UE_MVVM_SET_PROPERTY_VALUE(VoteHelpText, newVoteHelpText);
}

FText UUDDealGeneralTabViewModel::GetVoteHelpText() const
{
	return VoteHelpText;
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

void UUDDealGeneralTabViewModel::SetExecuteContractText(FText newExecuteContractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ExecuteContractText, newExecuteContractText);
}

FText UUDDealGeneralTabViewModel::GetExecuteContractText() const
{
	return ExecuteContractText;
}

void UUDDealGeneralTabViewModel::SetEndResolutionText(FText newEndResolutionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(EndResolutionText, newEndResolutionText);
}

FText UUDDealGeneralTabViewModel::GetEndResolutionText() const
{
	return EndResolutionText;
}

void UUDDealGeneralTabViewModel::SetEndVoteText(FText newEndVoteText)
{
	UE_MVVM_SET_PROPERTY_VALUE(EndVoteText, newEndVoteText);
}

FText UUDDealGeneralTabViewModel::GetEndVoteText() const
{
	return EndVoteText;
}

void UUDDealGeneralTabViewModel::SetEndDefineText(FText newEndDefineText)
{
	UE_MVVM_SET_PROPERTY_VALUE(EndDefineText, newEndDefineText);
}

FText UUDDealGeneralTabViewModel::GetEndDefineText() const
{
	return EndDefineText;
}

void UUDDealGeneralTabViewModel::SetEndAssembleText(FText newEndAssembleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(EndAssembleText, newEndAssembleText);
}

FText UUDDealGeneralTabViewModel::GetEndAssembleText() const
{
	return EndAssembleText;
}

void UUDDealGeneralTabViewModel::SetHelpTitleText(FText newHelpTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(HelpTitleText, newHelpTitleText);
}

FText UUDDealGeneralTabViewModel::GetHelpTitleText() const
{
	return HelpTitleText;
}

void UUDDealGeneralTabViewModel::SetHelpText(FText newHelpText)
{
	UE_MVVM_SET_PROPERTY_VALUE(HelpText, newHelpText);
}

FText UUDDealGeneralTabViewModel::GetHelpText() const
{
	return HelpText;
}

void UUDDealGeneralTabViewModel::SetCanExecuteContractValue(bool newCanExecuteContractValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanExecuteContractValue, newCanExecuteContractValue);
}

bool UUDDealGeneralTabViewModel::GetCanExecuteContractValue() const
{
	return CanExecuteContractValue;
}

void UUDDealGeneralTabViewModel::SetCanEndResolutionValue(bool newCanEndResolutionValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanEndResolutionValue, newCanEndResolutionValue);
}

bool UUDDealGeneralTabViewModel::GetCanEndResolutionValue() const
{
	return CanEndResolutionValue;
}

void UUDDealGeneralTabViewModel::SetCanEndVoteValue(bool newCanEndVoteValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanEndVoteValue, newCanEndVoteValue);
}

bool UUDDealGeneralTabViewModel::GetCanEndVoteValue() const
{
	return CanEndVoteValue;
}

void UUDDealGeneralTabViewModel::SetCanEndDefineValue(bool newCanEndDefineValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanEndDefineValue, newCanEndDefineValue);
}

bool UUDDealGeneralTabViewModel::GetCanEndDefineValue() const
{
	return CanEndDefineValue;
}

void UUDDealGeneralTabViewModel::SetCanEndAssembleValue(bool newCanEndAssembleValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanEndAssembleValue, newCanEndAssembleValue);
}

bool UUDDealGeneralTabViewModel::GetCanEndAssembleValue() const
{
	return CanEndAssembleValue;
}

void UUDDealGeneralTabViewModel::SetCanCloseDealValue(bool newCanCloseDealValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanCloseDealValue, newCanCloseDealValue);
}

bool UUDDealGeneralTabViewModel::GetCanCloseDealValue() const
{
	return CanCloseDealValue;
}