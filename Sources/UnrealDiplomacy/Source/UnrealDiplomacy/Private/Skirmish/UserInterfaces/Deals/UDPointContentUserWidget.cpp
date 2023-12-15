// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPointContentUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Components/Button.h"

void UUDPointContentUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPointContentViewModel* actionItemViewModel = CastChecked<UUDPointContentViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPointContentUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	IgnoreButtonWidget->OnClicked.Clear();
	IgnoreButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDPointContentViewModel::Ignore);
}

void UUDPointContentUserWidget::BindWidgets()
{
	IgnoreButtonWidget = GetWidget<UButton>(TEXT("IgnoreButton"));
}