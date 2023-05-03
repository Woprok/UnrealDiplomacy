// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDNewsWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Menu/UserInterfaces/UDNewsViewModel.h"

void UUDNewsWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDNewsViewModel* newsViewModel = CastChecked<UUDNewsViewModel>(viewModel.Get());
	ViewModel = newsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDNewsWidget::BindDelegates()
{
	CloseButton->OnClicked.AddUniqueDynamic(this, &UUDNewsWidget::Close);
}

void UUDNewsWidget::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDViewModel: Close."));
}