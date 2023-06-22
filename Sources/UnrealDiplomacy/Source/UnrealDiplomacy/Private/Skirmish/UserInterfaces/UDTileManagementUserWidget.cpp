// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDTileManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ListView.h"

void UUDTileManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDTileManagementViewModel* tileManagementViewModel = CastChecked<UUDTileManagementViewModel>(viewModel.Get());
	ViewModel = tileManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDTileManagementUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->TileInteractionSourceUpdatedEvent.AddUObject(this, &UUDTileManagementUserWidget::SetTileInteractionSourceCollection);
	ViewModel->ModifierItemSourceUpdatedEvent.AddUObject(this, &UUDTileManagementUserWidget::SetModifierItemSourceCollection);
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
	TileInteractionListWidget = GetWidget<UListView>(TEXT("TileInteractionList"));
	ModifierItemListWidget = GetWidget<UListView>(TEXT("ModifierItemList"));
}

void UUDTileManagementUserWidget::SetTileInteractionSourceCollection(const TArray<TObjectPtr<UUDTileInteractionViewModel>>& tileInteractionViewModels)
{
	TileInteractionListWidget->SetListItems(tileInteractionViewModels);
}

void UUDTileManagementUserWidget::SetModifierItemSourceCollection(const TArray<TObjectPtr<UUDModifierItemViewModel>>& modifierItemViewModels)
{
	ModifierItemListWidget->SetListItems(modifierItemViewModels);
}