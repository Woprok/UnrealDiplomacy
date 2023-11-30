// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDTileManagementViewModel.h"
#include "Components/Button.h"

void UUDTileManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTileManagementViewModel* tileManagementViewModel = CastChecked<UUDTileManagementViewModel>(viewModel.Get());
	ViewModel = tileManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTileManagementUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTileManagementViewModel::Close);
}

void UUDTileManagementUserWidget::BindWidgets()
{
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
}