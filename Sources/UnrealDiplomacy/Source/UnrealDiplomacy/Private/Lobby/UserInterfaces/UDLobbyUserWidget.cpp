// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDLobbyUserWidget.h"
#include "Lobby/UserInterfaces/UDLobbyViewModel.h"
#include "Lobby/UserInterfaces/UDLobbyHostViewModel.h"
#include "Lobby/UserInterfaces/UDLobbyMemberViewModel.h"
#include "Lobby/UserInterfaces/UDLobbyHostUserWidget.h"
#include "Lobby/UserInterfaces/UDLobbyMemberUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDLobbyUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDLobbyViewModel* lobbyViewModel = CastChecked<UUDLobbyViewModel>(viewModel.Get());
	ViewModel = lobbyViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDLobbyUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->LobbyHostSourceChangedEvent.AddUObject(this, &UUDLobbyUserWidget::SetHostSourceInstance);
	ViewModel->LobbyMemberSourceChangedEvent.AddUObject(this, &UUDLobbyUserWidget::SetMemberSourceInstance);
	// Bind viewmodel to widgets.
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyViewModel::Back);
	LeaveButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyViewModel::Leave);
	StartButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDLobbyViewModel::Start);
}

void UUDLobbyUserWidget::BindWidgets()
{
	LobbyTitleTextWidget = GetWidget<UTextBlock>(TEXT("LobbyTitleText"));
	NameHeaderTextWidget = GetWidget<UTextBlock>(TEXT("NameHeaderText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	LeaveTextWidget = GetWidget<UTextBlock>(TEXT("LeaveText"));
	StartTextWidget = GetWidget<UTextBlock>(TEXT("StartText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
	LeaveButtonWidget = GetWidget<UButton>(TEXT("LeaveButton"));
	StartButtonWidget = GetWidget<UButton>(TEXT("StartButton"));
	LobbyHostViewWidget = GetWidget<UUDLobbyHostUserWidget>(TEXT("LobbyHostView"));
	LobbyMemberViewWidget = GetWidget<UUDLobbyMemberUserWidget>(TEXT("LobbyMemberView"));
	ClientItemListWidget = GetWidget<UUDListView>(TEXT("ClientItemList"));
}

void UUDLobbyUserWidget::SetHostSourceInstance(const TObjectPtr<UUDLobbyHostViewModel>& lobbyHostViewModel)
{
	LobbyHostViewWidget->SetViewModel(lobbyHostViewModel);
}

void UUDLobbyUserWidget::SetMemberSourceInstance(const TObjectPtr<UUDLobbyMemberViewModel>& lobbyMemberViewModel)
{
	LobbyMemberViewWidget->SetViewModel(lobbyMemberViewModel);
}