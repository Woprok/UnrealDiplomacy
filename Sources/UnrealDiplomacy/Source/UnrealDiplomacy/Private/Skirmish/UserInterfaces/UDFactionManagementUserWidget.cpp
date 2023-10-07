// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ListView.h"

void UUDFactionManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionManagementViewModel* factionManagementViewModel = CastChecked<UUDFactionManagementViewModel>(viewModel.Get());
	ViewModel = factionManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionManagementUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->FactionInteractionSourceUpdatedEvent.AddUObject(this, &UUDFactionManagementUserWidget::SetFactionInteractionSourceCollection);
	ViewModel->FactionOfferSourceUpdatedEvent.AddUObject(this, &UUDFactionManagementUserWidget::SetFactionOfferSourceCollection);
	ViewModel->FactionRequestSourceUpdatedEvent.AddUObject(this, &UUDFactionManagementUserWidget::SetFactionRequestSourceCollection);
	ViewModel->FactionDemandSourceUpdatedEvent.AddUObject(this, &UUDFactionManagementUserWidget::SetFactionDemandSourceCollection);
	ViewModel->ModifierItemSourceUpdatedEvent.AddUObject(this, &UUDFactionManagementUserWidget::SetModifierItemSourceCollection);
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionManagementViewModel::Close);
}

void UUDFactionManagementUserWidget::BindWidgets()
{
	FactionManagementTitleTextWidget = GetWidget<UTextBlock>(TEXT("FactionManagementTitleText"));
	FactionNameTextWidget = GetWidget<UTextBlock>(TEXT("FactionNameText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
	InteractionTitleTextWidget = GetWidget<UTextBlock>(TEXT("InteractionTitleText"));
	OfferTitleTextWidget = GetWidget<UTextBlock>(TEXT("OfferTitleText"));
	RequestTitleTextWidget = GetWidget<UTextBlock>(TEXT("RequestTitleText"));
	DemandTitleTextWidget = GetWidget<UTextBlock>(TEXT("DemandTitleText"));
	FactionInteractionListWidget = GetWidget<UListView>(TEXT("FactionInteractionList"));
	FactionOfferListWidget = GetWidget<UListView>(TEXT("FactionOfferList"));
	FactionRequestListWidget = GetWidget<UListView>(TEXT("FactionRequestList"));
	FactionDemandListWidget = GetWidget<UListView>(TEXT("FactionDemandList"));
	ModifierItemListWidget = GetWidget<UListView>(TEXT("ModifierItemList"));
}

void UUDFactionManagementUserWidget::SetFactionInteractionSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionInteractionViewModels)
{
	FactionInteractionListWidget->SetListItems(factionInteractionViewModels);
}

void UUDFactionManagementUserWidget::SetFactionOfferSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionOfferViewModels)
{
	FactionOfferListWidget->SetListItems(factionOfferViewModels);
}

void UUDFactionManagementUserWidget::SetFactionRequestSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionRequestViewModels)
{
	FactionRequestListWidget->SetListItems(factionRequestViewModels);
}

void UUDFactionManagementUserWidget::SetFactionDemandSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionDemandViewModels)
{
	FactionDemandListWidget->SetListItems(factionDemandViewModels);
}

void UUDFactionManagementUserWidget::SetModifierItemSourceCollection(const TArray<TObjectPtr<UUDModifierItemViewModel>>& modifierItemViewModels)
{
	ModifierItemListWidget->SetListItems(modifierItemViewModels);
}