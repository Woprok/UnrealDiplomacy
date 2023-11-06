// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDTileManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDTileManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTileManagementViewModel* tileManagementViewModel = CastChecked<UUDTileManagementViewModel>(viewModel.Get());
	ViewModel = tileManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTileManagementUserWidget::BindDelegates()
{
	// Bind view to updates.
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDTileManagementViewModel::Close);
}

void UUDTileManagementUserWidget::BindWidgets()
{
	TileManagementTitleTextWidget = GetWidget<UTextBlock>(TEXT("TileManagementTitleText"));
	TileNameTextWidget = GetWidget<UTextBlock>(TEXT("TileNameText"));
	OwnerTextWidget = GetWidget<UTextBlock>(TEXT("OwnerText"));
	FactionNameTextWidget = GetWidget<UTextBlock>(TEXT("FactionNameText"));
	ResourceTextWidget = GetWidget<UTextBlock>(TEXT("ResourceText"));
	ResourceValueTextWidget = GetWidget<UTextBlock>(TEXT("ResourceValueText"));
	ResourceTypeTextWidget = GetWidget<UTextBlock>(TEXT("ResourceTypeText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
	TileInteractionListWidget = GetWidget<UUDListView>(TEXT("TileInteractionList"));
	ModifierItemListWidget = GetWidget<UUDListView>(TEXT("ModifierItemList"));
}