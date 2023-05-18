// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDNewsWidget.h"
#include "Menu/UserInterfaces/UDNewsViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDNewsWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDNewsViewModel* newsViewModel = CastChecked<UUDNewsViewModel>(viewModel.Get());
	ViewModel = newsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDNewsWidget::BindWidgets()
{
	TitleTextWidget = GetWidget<UTextBlock>(TEXT("TitleText"));
	MessageTextWidget = GetWidget<UTextBlock>(TEXT("MessageText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
}

void UUDNewsWidget::BindDelegates()
{
	CloseButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDNewsWidget::Close);
}

void UUDNewsWidget::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDNewsWidget: Close."));
	RemoveFromParent();
}