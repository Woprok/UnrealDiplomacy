// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionManagementViewModel.h"
#include "Components/Button.h"

void UUDFactionManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionManagementViewModel* factionManagementViewModel = CastChecked<UUDFactionManagementViewModel>(viewModel.Get());
	ViewModel = factionManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionManagementUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.Clear();
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionManagementViewModel::Close);
}

void UUDFactionManagementUserWidget::BindWidgets()
{
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
}