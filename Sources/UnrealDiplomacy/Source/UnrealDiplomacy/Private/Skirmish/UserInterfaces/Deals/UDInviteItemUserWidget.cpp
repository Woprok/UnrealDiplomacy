// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDInviteItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDInviteItemViewModel.h"
#include "Components/Button.h"

void UUDInviteItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDInviteItemViewModel* actionItemViewModel = CastChecked<UUDInviteItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDInviteItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	InviteButtonWidget->OnClicked.Clear();
	InviteButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDInviteItemViewModel::Invite);
}

void UUDInviteItemUserWidget::BindWidgets()
{
	InviteButtonWidget = GetWidget<UButton>(TEXT("InviteButton"));
}