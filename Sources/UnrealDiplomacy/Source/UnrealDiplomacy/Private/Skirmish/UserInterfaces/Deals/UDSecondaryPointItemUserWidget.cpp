// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDSecondaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDTertiaryPointItemViewModel.h"
#include "Components/ListView.h"
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
	ViewModel->PointItemSourceUpdatedEvent.AddUObject(this, &UUDSecondaryPointItemUserWidget::SetPointItemSourceCollection);
	// Bind viewmodel to widgets.
	CreateSecondaryPointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDSecondaryPointItemViewModel::CreateSecondaryPoint);
}

void UUDSecondaryPointItemUserWidget::BindWidgets()
{
	CreateSecondaryPointTextWidget = GetWidget<UTextBlock>(TEXT("CreateSecondaryPointText"));
	CreateSecondaryPointButtonWidget = GetWidget<UButton>(TEXT("CreateSecondaryPointButton"));
	PointItemListWidget = GetWidget<UListView>(TEXT("PointItemList"));
}

void UUDSecondaryPointItemUserWidget::SetPointItemSourceCollection(const TArray<TObjectPtr<UUDTertiaryPointItemViewModel>>& itemViewModels)
{
	PointItemListWidget->SetListItems(itemViewModels);
}