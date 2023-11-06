// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDPrimaryPointItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPrimaryPointItemViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDPointContentViewModel.h"
#include "Core/UserInterfaces/Components/UDListView.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDPrimaryPointItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDPrimaryPointItemViewModel* itemViewModel = Cast<UUDPrimaryPointItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDPrimaryPointItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDPrimaryPointItemViewModel* actionItemViewModel = CastChecked<UUDPrimaryPointItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDPrimaryPointItemUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->PointContentSourceUpdatedEvent.AddUObject(this, &UUDPrimaryPointItemUserWidget::SetPointContentSourceCollection);
	// Bind viewmodel to widgets.
	CreatePointButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDPrimaryPointItemViewModel::CreatePoint);
}

void UUDPrimaryPointItemUserWidget::BindWidgets()
{
	CreatePointTextWidget = GetWidget<UTextBlock>(TEXT("CreatePointText"));
	CreatePointButtonWidget = GetWidget<UButton>(TEXT("CreatePointButton"));
	PointItemListWidget = GetWidget<UUDListView>(TEXT("PointItemList"));
	PointContentViewWidget = GetWidget<UUDPointContentUserWidget>(TEXT("PointContentView"));
}

void UUDPrimaryPointItemUserWidget::SetPointContentSourceCollection(const TObjectPtr<UUDPointContentViewModel>& itemViewModel)
{
	PointContentViewWidget->SetViewModel(itemViewModel);
}