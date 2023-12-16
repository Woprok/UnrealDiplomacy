// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDDealGeneralTabViewModel.h"
#include "Components/Button.h"

void UUDDealGeneralTabUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDealGeneralTabViewModel* dealItemViewModel = CastChecked<UUDDealGeneralTabViewModel>(viewModel.Get());
	ViewModel = dealItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDealGeneralTabUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	ChangeReadyButtonWidget->OnClicked.Clear();
	ChangeVoteButtonWidget->OnClicked.Clear();
	LeaveButtonWidget->OnClicked.Clear();
	CancelButtonWidget->OnClicked.Clear();

	EndAssembleButtonWidget->OnClicked.Clear();
	EndDefineButtonWidget->OnClicked.Clear();
	EndVoteButtonWidget->OnClicked.Clear();
	EndResolutionButtonWidget->OnClicked.Clear();
	ExecuteContractButtonWidget->OnClicked.Clear();

	ChangeReadyButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ChangeReady);
	ChangeVoteButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ChangeVote);
	LeaveButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::Leave);
	CancelButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::Cancel);

	EndAssembleButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::EndAssemble);
	EndDefineButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::EndDefine);
	EndVoteButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::EndVote);
	EndResolutionButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::EndResolution);
	ExecuteContractButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ExecuteContract);
}

void UUDDealGeneralTabUserWidget::BindWidgets()
{
	ChangeReadyButtonWidget = GetWidget<UButton>(TEXT("ChangeReadyButton"));
	ChangeVoteButtonWidget = GetWidget<UButton>(TEXT("ChangeVoteButton"));
	LeaveButtonWidget = GetWidget<UButton>(TEXT("LeaveButton"));
	CancelButtonWidget = GetWidget<UButton>(TEXT("CancelButton"));

	EndAssembleButtonWidget = GetWidget<UButton>(TEXT("EndAssembleButton"));
	EndDefineButtonWidget = GetWidget<UButton>(TEXT("EndDefineButton"));
	EndVoteButtonWidget = GetWidget<UButton>(TEXT("EndVoteButton"));
	EndResolutionButtonWidget = GetWidget<UButton>(TEXT("EndResolutionButton"));
	ExecuteContractButtonWidget = GetWidget<UButton>(TEXT("ExecuteContractButton"));
}