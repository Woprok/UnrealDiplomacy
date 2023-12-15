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

	CreateContractButtonWidget->OnClicked.Clear();
	ExecuteContractButtonWidget->OnClicked.Clear();

	ChangeReadyButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ChangeReady);
	ChangeVoteButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ChangeVote);
	LeaveButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::Leave);
	CancelButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::Cancel);

	CreateContractButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::CreateContract);
	ExecuteContractButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDealGeneralTabViewModel::ExecuteContract);
}

void UUDDealGeneralTabUserWidget::BindWidgets()
{
	ChangeReadyButtonWidget = GetWidget<UButton>(TEXT("ChangeReadyButton"));
	ChangeVoteButtonWidget = GetWidget<UButton>(TEXT("ChangeVoteButton"));
	LeaveButtonWidget = GetWidget<UButton>(TEXT("LeaveButton"));
	CancelButtonWidget = GetWidget<UButton>(TEXT("CancelButton"));

	CreateContractButtonWidget = GetWidget<UButton>(TEXT("CreateContractButton"));
	ExecuteContractButtonWidget = GetWidget<UButton>(TEXT("ExecuteContractButton"));
}