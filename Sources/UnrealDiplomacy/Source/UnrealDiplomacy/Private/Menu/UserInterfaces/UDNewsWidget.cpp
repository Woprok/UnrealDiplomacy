// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDNewsWidget.h"
#include "Menu/UserInterfaces/UDNewsViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDNewsWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDNewsViewModel* newsViewModel = CastChecked<UUDNewsViewModel>(viewModel.Get());
	ViewModel = newsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDNewsWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDNewsViewModel::Close);
}

void UUDNewsWidget::BindWidgets()
{
	NewsTitleTextWidget = GetWidget<UTextBlock>(TEXT("NewsTitleText"));
	MessageTextWidget = GetWidget<UTextBlock>(TEXT("MessageText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
}