// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDDialogueUserWidget.h"
#include "Menu/UserInterfaces/UDDialogueViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UUDDialogueUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDDialogueViewModel* dialogueViewModel = CastChecked<UUDDialogueViewModel>(viewModel.Get());
	ViewModel = dialogueViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDDialogueUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDDialogueViewModel::Close);
}

void UUDDialogueUserWidget::BindWidgets()
{
	DialogueTitleTextWidget = GetWidget<UTextBlock>(TEXT("DialogueTitleText"));
	ContentTextWidget = GetWidget<UTextBlock>(TEXT("ContentText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
}