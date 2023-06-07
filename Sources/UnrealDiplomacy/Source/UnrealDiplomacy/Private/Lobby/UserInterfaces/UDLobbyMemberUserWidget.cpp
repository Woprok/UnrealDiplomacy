// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyMemberUserWidget.h"
#include "Lobby/UserInterfaces/UDLobbyMemberViewModel.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/TileView.h"

void UUDLobbyMemberUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLobbyMemberViewModel* lobbyMemberViewModel = CastChecked<UUDLobbyMemberViewModel>(viewModel.Get());
	ViewModel = lobbyMemberViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDLobbyMemberUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	FactionNameEditableTextWidget->OnTextChanged.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyMemberViewModel::StartNameEditation);
	FactionNameEditableTextWidget->OnTextCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyMemberViewModel::StopNameEditation);
}

void UUDLobbyMemberUserWidget::BindWidgets()
{
	MemberSettingsTitleTextWidget = GetWidget<UTextBlock>(TEXT("MemberSettingsTitleText"));
	FactionNameTextWidget = GetWidget<UTextBlock>(TEXT("FactionNameText"));
	StrategyTextWidget = GetWidget<UTextBlock>(TEXT("StrategyText"));
	FactionNameEditableTextWidget = GetWidget<UEditableTextBox>(TEXT("FactionNameEditableText"));
	StrategyOptionTilesWidget = GetWidget<UTileView>(TEXT("StrategyOptionTiles"));
}