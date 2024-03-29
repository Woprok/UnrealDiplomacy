// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Deals/UDChatUserWidget.h"
#include "Skirmish/UserInterfaces/Deals/UDChatViewModel.h"
#include "Skirmish/UserInterfaces/Deals/UDChatItemViewModel.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"

void UUDChatUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDChatViewModel* actionItemViewModel = CastChecked<UUDChatViewModel>(viewModel.Get());
	ViewModel = actionItemViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDChatUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
	EditableMessageTextBoxWidget->OnTextChanged.Clear();
	EditableMessageTextBoxWidget->OnTextCommitted.Clear();
	EditableMessageTextBoxWidget->OnTextChanged.AddUniqueDynamic(ViewModel.Get(), &UUDChatViewModel::StartTextEditation);
	EditableMessageTextBoxWidget->OnTextCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDChatViewModel::StopTextEditation);

	SendButtonWidget->OnClicked.Clear();
	SendButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDChatViewModel::Send);
}

void UUDChatUserWidget::BindWidgets()
{
	EditableMessageTextBoxWidget = GetWidget<UMultiLineEditableTextBox>(TEXT("EditableMessageTextBox"));
	SendButtonWidget = GetWidget<UButton>(TEXT("SendButton"));
}