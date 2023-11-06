// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDPolicySelectorUserWidget.h"
#include "Skirmish/UserInterfaces/UDPolicySelectorViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/UserInterfaces/Components/UDListView.h"

void UUDFactionManagementUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDFactionManagementViewModel* factionManagementViewModel = CastChecked<UUDFactionManagementViewModel>(viewModel.Get());
	ViewModel = factionManagementViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDFactionManagementUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->PolicySelectorChangedEvent.AddUObject(this, &UUDFactionManagementUserWidget::SetPolicySelectorSourceInstance);
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
	FactionInteractionListWidget = GetWidget<UUDListView>(TEXT("FactionInteractionList"));
	FactionOfferListWidget = GetWidget<UUDListView>(TEXT("FactionOfferList"));
	FactionRequestListWidget = GetWidget<UUDListView>(TEXT("FactionRequestList"));
	FactionDemandListWidget = GetWidget<UUDListView>(TEXT("FactionDemandList"));
	ModifierItemListWidget = GetWidget<UUDListView>(TEXT("ModifierItemList"));
	PolicySelectorViewWidget = GetWidget<UUDPolicySelectorUserWidget>(TEXT("PolicySelectorView"));
}

void UUDFactionManagementUserWidget::SetPolicySelectorSourceInstance(const TObjectPtr<UUDPolicySelectorViewModel>& policySelectorViewModel)
{
	PolicySelectorViewWidget->SetViewModel(policySelectorViewModel);
}