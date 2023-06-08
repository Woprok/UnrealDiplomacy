// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyHostUserWidget.h"
#include "Lobby/UserInterfaces/UDLobbyHostViewModel.h"
#include "Components/TextBlock.h"
#include "Components/SpinBox.h"

void UUDLobbyHostUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLobbyHostViewModel* lobbyHostViewModel = CastChecked<UUDLobbyHostViewModel>(viewModel.Get());
	ViewModel = lobbyHostViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDLobbyHostUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	AICountValueWidget->OnValueChanged.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StartValueEditation);
	AICountValueWidget->OnValueCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StopAICountValueEditation);
	MapSeedValueWidget->OnValueChanged.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StartValueEditation);
	MapSeedValueWidget->OnValueCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StopMapSeedValueEditation);
	MapWidthValueWidget->OnValueChanged.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StartValueEditation);
	MapWidthValueWidget->OnValueCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StopMapWidthValueEditation);
	MapHeightValueWidget->OnValueChanged.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StartValueEditation);
	MapHeightValueWidget->OnValueCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StopMapHeightValueEditation);
	StratagemPointsValueWidget->OnValueChanged.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StartValueEditation);
	StratagemPointsValueWidget->OnValueCommitted.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyHostViewModel::StopStratagemPointsValueEditation);
}

void UUDLobbyHostUserWidget::BindWidgets()
{
	HostSettingsTitleTextWidget = GetWidget<UTextBlock>(TEXT("HostSettingsTitleText"));
	AICountTextWidget = GetWidget<UTextBlock>(TEXT("AICountText"));
	MapSeedTextWidget = GetWidget<UTextBlock>(TEXT("MapSeedText"));
	MapWidthTextWidget = GetWidget<UTextBlock>(TEXT("MapWidthText"));
	MapHeightTextWidget = GetWidget<UTextBlock>(TEXT("MapHeightText"));
	StratagemPointsTextWidget = GetWidget<UTextBlock>(TEXT("StratagemPointsText"));
	AICountValueWidget = GetWidget<USpinBox>(TEXT("AICountValue"));
	MapSeedValueWidget = GetWidget<USpinBox>(TEXT("MapSeedValue"));
	MapWidthValueWidget = GetWidget<USpinBox>(TEXT("MapWidthValue"));
	MapHeightValueWidget = GetWidget<USpinBox>(TEXT("MapHeightValue"));
	StratagemPointsValueWidget = GetWidget<USpinBox>(TEXT("StratagemPointsValue"));
}