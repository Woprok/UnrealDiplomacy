// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyMemberUserWidget.h"
#include "Lobby/UserInterfaces/UDLobbyMemberViewModel.h"
#include "Components/EditableTextBox.h"

void UUDLobbyMemberUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLobbyMemberViewModel* lobbyMemberViewModel = CastChecked<UUDLobbyMemberViewModel>(viewModel.Get());
	ViewModel = lobbyMemberViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDLobbyMemberUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	FactionNameEditableTextWidget->OnTextChanged.Clear();
	FactionNameEditableTextWidget->OnTextCommitted.Clear();
	FactionNameEditableTextWidget->OnTextChanged.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyMemberViewModel::StartNameEditation);
	FactionNameEditableTextWidget->OnTextCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyMemberViewModel::StopNameEditation);
}

void UUDLobbyMemberUserWidget::BindWidgets()
{
	FactionNameEditableTextWidget = GetWidget<UEditableTextBox>(TEXT("FactionNameEditableText"));
}