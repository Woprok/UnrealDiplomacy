// Copyright Miroslav Valach

#include "Core/UserInterfaces/UDUserWidget.h"

void UUDUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel) 
{
	// Abstract implementation cannot store this reference.
	return;
}

void UUDUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindDelegates();
	SetAppearance();
}

void UUDUserWidget::BindDelegates()
{
	return;
}

void UUDUserWidget::SetAppearance()
{
	return;
}