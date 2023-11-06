// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Core/UserInterfaces/Components/UDListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDSecondaryPointItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDSecondaryPointItemViewModel* itemViewModel = Cast<UUDSecondaryPointItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDSecondaryPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDSecondaryPointItemViewModel* actionItemViewModel = CastChecked<UUDSecondaryPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDSecondaryPointItemUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->PointContentSourceUpdatedEvent.AddUObject(this, &UUDSecondaryPointItemUserWidget::SetPointContentSourceCollection);
	// Bind viewmodel to widgets.
	CreateSecondaryPointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDSecondaryPointItemViewModel::CreateSecondaryPoint);
}

void UUDSecondaryPointItemUserWidget::BindWidgets()
{
	CreateSecondaryPointTextWidget = GetWidget<UTextBlock>(TEXT("CreateSecondaryPointText"));
	CreateSecondaryPointButtonWidget = GetWidget<UButton>(TEXT("CreateSecondaryPointButton"));
	PointItemListWidget = GetWidget<UUDListView>(TEXT("PointItemList"));
	PointContentViewWidget = GetWidget<UUDPointContentUserWidget>(TEXT("PointContentView"));
}

void UUDSecondaryPointItemUserWidget::SetPointContentSourceCollection(const TObjectPtr<UUDPointContentViewModel>& itemViewModel)
{
	PointContentViewWidget->SetViewModel(itemViewModel);
}