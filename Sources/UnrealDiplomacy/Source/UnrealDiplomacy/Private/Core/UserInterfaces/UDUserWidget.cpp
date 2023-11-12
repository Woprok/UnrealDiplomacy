// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDUserWidget.h"
#include "Core/UserInterfaces/UDViewModel.h"

void UUDUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel) 
{
	BindViewModel(viewModel);
	BindDelegates();
}

void UUDUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BindWidgets();
}

void UUDUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UUDUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	return;
}

void UUDUserWidget::BindDelegates()
{
	return;
}

void UUDUserWidget::BindWidgets()
{
	return;
}