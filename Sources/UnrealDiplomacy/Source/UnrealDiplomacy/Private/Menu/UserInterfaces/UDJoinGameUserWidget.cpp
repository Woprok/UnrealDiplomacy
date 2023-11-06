// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDJoinGameUserWidget.h"
#include "Menu/UserInterfaces/UDJoinGameViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDJoinGameUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDJoinGameViewModel* joinGameViewModel = CastChecked<UUDJoinGameViewModel>(viewModel.Get());
	ViewModel = joinGameViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDJoinGameUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDJoinGameViewModel::Back);
	RefreshButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDJoinGameViewModel::Refresh);
}

void UUDJoinGameUserWidget::BindWidgets()
{
	JoinGameTitleTextWidget = GetWidget<UTextBlock>(TEXT("JoinGameTitleText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	RefreshTextWidget = GetWidget<UTextBlock>(TEXT("RefreshText"));
	NameHeaderTextWidget = GetWidget<UTextBlock>(TEXT("NameHeaderText"));
	PingHeaderTextWidget = GetWidget<UTextBlock>(TEXT("PingHeaderText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
	RefreshButtonWidget = GetWidget<UButton>(TEXT("RefreshButton"));
	ServerItemListWidget = GetWidget<UUDListView>(TEXT("ServerItemList"));
}