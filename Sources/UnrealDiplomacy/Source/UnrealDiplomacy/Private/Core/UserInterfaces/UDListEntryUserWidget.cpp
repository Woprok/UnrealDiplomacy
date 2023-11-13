// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDListEntryUserWidget.h"
#include "Core/UserInterfaces/UDViewModel.h"

void UUDListEntryUserWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Change it to original type.
	UUDViewModel* itemViewModel = Cast<UUDViewModel>(ListItemObject);
	// Invoke bindings related to view model being set.
	SetViewModel(itemViewModel);
}