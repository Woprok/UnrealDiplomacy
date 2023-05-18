// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDUserWidget.h"

void UUDUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel) 
{
	// Abstract implementation cannot store this reference.
	return;
}

void UUDUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BindWidgets();
	BindDelegates();
}

void UUDUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetAppearance();
}

void UUDUserWidget::BindDelegates()
{
	return;
}

void UUDUserWidget::BindWidgets()
{
	return;
}

void UUDUserWidget::SetAppearance()
{
	return;
}