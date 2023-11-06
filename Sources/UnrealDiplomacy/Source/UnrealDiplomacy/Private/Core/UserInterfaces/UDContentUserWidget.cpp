// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "Core/UserInterfaces/UDViewModel.h"

FUDViewModelContent UUDContentUserWidget::GetViewModelContent() const
{
	return ViewModelContent;
}

void UUDContentUserWidget::SetViewModelContent(const FUDViewModelContent& content)
{
	ViewModelContent = content;
	SetViewModel(content.ViewModel);
}