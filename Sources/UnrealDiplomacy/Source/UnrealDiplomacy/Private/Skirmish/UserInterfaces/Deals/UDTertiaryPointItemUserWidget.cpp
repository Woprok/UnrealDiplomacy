// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemViewModel.h"
#include "Components/Button.h"

void UUDTertiaryPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTertiaryPointItemViewModel* actionItemViewModel = CastChecked<UUDTertiaryPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTertiaryPointItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CreateTertiaryPointButtonWidget->OnClicked.Clear();
	CreateTertiaryPointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTertiaryPointItemViewModel::CreateTertiaryPoint);
}

void UUDTertiaryPointItemUserWidget::BindWidgets()
{
	CreateTertiaryPointButtonWidget = GetWidget<UButton>(TEXT("CreateTertiaryPointButton"));
}