// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDCreditsUserWidget.h"
#include "Menu/UserInterfaces/UDCreditsViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDCreditsUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDCreditsViewModel* creditsViewModel = CastChecked<UUDCreditsViewModel>(viewModel.Get());
	ViewModel = creditsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDCreditsUserWidget::BindDelegates()
{
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDCreditsViewModel::Back);
}

void UUDCreditsUserWidget::BindWidgets()
{
	// Bind viewmodel to widgets.
	CreditsTitleTextWidget = GetWidget<UTextBlock>(TEXT("CreditsTitleText"));
	ContentTextWidget = GetWidget<UTextBlock>(TEXT("ContentText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
}