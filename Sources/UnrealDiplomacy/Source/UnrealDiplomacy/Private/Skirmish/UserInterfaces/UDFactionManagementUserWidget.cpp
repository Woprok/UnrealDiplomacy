// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionManagementUserWidget.h"
#include "Skirmish/UserInterfaces/UDFactionManagementViewModel.h"
#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
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
	// Bind viewmodel to widgets.
	CloseButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDFactionManagementViewModel::Close);
}

void UUDFactionManagementUserWidget::BindWidgets()
{
	FactionManagementTitleTextWidget = GetWidget<UTextBlock>(TEXT("FactionManagementTitleText"));
	FactionNameTextWidget = GetWidget<UTextBlock>(TEXT("FactionNameText"));
	CloseTextWidget = GetWidget<UTextBlock>(TEXT("CloseText"));
	CloseButtonWidget = GetWidget<UButton>(TEXT("CloseButton"));
	FactionInteractionListWidget = GetWidget<UListView>(TEXT("FactionInteractionList"));
}

void UUDFactionManagementUserWidget::SetFactionInteractionSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionInteractionViewModels)
{
	FactionInteractionListWidget->SetListItems(factionInteractionViewModels);
}