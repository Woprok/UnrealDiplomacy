// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDLoadingUserWidget.h"
#include "Menu/UserInterfaces/UDLoadingViewModel.h"
#include "Components/TextBlock.h"

void UUDLoadingUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLoadingViewModel* loadingViewModel = CastChecked<UUDLoadingViewModel>(viewModel.Get());
	ViewModel = loadingViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDLoadingUserWidget::BindWidgets()
{
	LoadingTitleTextWidget = GetWidget<UTextBlock>(TEXT("LoadingTitleText"));
	GameplayHintTextWidget = GetWidget<UTextBlock>(TEXT("GameplayHintText"));
}