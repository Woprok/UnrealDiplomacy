// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyHostUserWidget.h"
#include "Lobby/UserInterfaces/UDLobbyHostViewModel.h"
#include "Components/TextBlock.h"

void UUDLobbyHostUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLobbyHostViewModel* lobbyHostViewModel = CastChecked<UUDLobbyHostViewModel>(viewModel.Get());
	ViewModel = lobbyHostViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDLobbyHostUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
}

void UUDLobbyHostUserWidget::BindWidgets()
{
	HostSettingsTitleTextWidget = GetWidget<UTextBlock>(TEXT("HostSettingsTitleText"));
	AICountTextWidget = GetWidget<UTextBlock>(TEXT("AICountText"));
	MapSeedTextWidget = GetWidget<UTextBlock>(TEXT("MapSeedText"));
	MapWidthTextWidget = GetWidget<UTextBlock>(TEXT("MapWidthText"));
	MapHeightTextWidget = GetWidget<UTextBlock>(TEXT("MapHeightText"));
}