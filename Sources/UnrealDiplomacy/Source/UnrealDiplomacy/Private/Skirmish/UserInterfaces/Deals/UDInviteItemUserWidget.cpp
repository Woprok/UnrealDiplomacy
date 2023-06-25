// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDInviteItemUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDInviteItemViewModel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUDInviteItemUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDInviteItemViewModel* itemViewModel = Cast<UUDInviteItemViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}

void UUDInviteItemUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDInviteItemViewModel* actionItemViewModel = CastChecked<UUDInviteItemViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDInviteItemUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDInviteItemUserWidget::BindWidgets()
{
}