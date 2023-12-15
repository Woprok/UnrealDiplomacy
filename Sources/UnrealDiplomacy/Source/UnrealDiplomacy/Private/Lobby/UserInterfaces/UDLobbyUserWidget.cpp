// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyUserWidget.h"
#include "Lobby/UserInterfaces/UDLobbyViewModel.h"
#include "Components/Button.h"

void UUDLobbyUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLobbyViewModel* lobbyViewModel = CastChecked<UUDLobbyViewModel>(viewModel.Get());
	ViewModel = lobbyViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDLobbyUserWidget::BindDelegates()
{
	// Bind viewmodel to widgets.
	BackButtonWidget->OnClicked.Clear();
	LeaveButtonWidget->OnClicked.Clear();
	StartButtonWidget->OnClicked.Clear();
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyViewModel::Back);
	LeaveButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyViewModel::Leave);
	StartButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyViewModel::Start);
}

void UUDLobbyUserWidget::BindWidgets()
{
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
	LeaveButtonWidget = GetWidget<UButton>(TEXT("LeaveButton"));
	StartButtonWidget = GetWidget<UButton>(TEXT("StartButton"));
}