// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "Core/UserInterfaces/UDViewModel.h"

FUDViewModelContent UUDContentUserWidget::GetViewModelContent() const
{
	return ViewModelContent;
}

void UUDContentUserWidget::SetViewModelContent(const FUDViewModelContent& content)
{
	// Content items are always present, once added to UI.
	// In some cases this could be invoked with null, thus it needs to be avoid.
	// Note: this null call is basically done by constructor, so it's quite pointless to stop it from firing.
	if (content.ViewModel == nullptr)
		return;

	ViewModelContent = content;
	SetViewModel(content.ViewModel);
}