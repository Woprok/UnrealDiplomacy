// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDTertiaryPointItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDTertiaryPointItemViewModel* itemViewModel = Cast<UUDTertiaryPointItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDTertiaryPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTertiaryPointItemViewModel* actionItemViewModel = CastChecked<UUDTertiaryPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTertiaryPointItemUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->PointContentSourceUpdatedEvent.AddUObject(this, &UUDTertiaryPointItemUserWidget::SetPointContentSourceCollection);
	// Bind viewmodel to widgets.
	CreateTertiaryPointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTertiaryPointItemViewModel::CreateTertiaryPoint);
}

void UUDTertiaryPointItemUserWidget::BindWidgets()
{
	CreateTertiaryPointTextWidget = GetWidget<UTextBlock>(TEXT("CreateTertiaryPointText"));
	CreateTertiaryPointButtonWidget = GetWidget<UButton>(TEXT("CreateTertiaryPointButton"));
	PointContentViewWidget = GetWidget<UUDPointContentUserWidget>(TEXT("PointContentView"));
}

void UUDTertiaryPointItemUserWidget::SetPointContentSourceCollection(const TObjectPtr<UUDPointContentViewModel>& itemViewModel)
{
	PointContentViewWidget->SetViewModel(itemViewModel);
}