// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDSettingsUserWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "Menu/UDMenuHUD.h"

void UUDSettingsUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDSettingsViewModel* settingsViewModel = CastChecked<UUDSettingsViewModel>(viewModel.Get());
	ViewModel = settingsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDSettingsUserWidget::BindDelegates()
{
	BackButton->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Back);
	SaveButton->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Save);
	WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &UUDSettingsUserWidget::WindowModeChanged);
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UUDSettingsUserWidget::ResolutionChanged);
}

void UUDSettingsUserWidget::Back()
{
	AUDMenuHUD::Get(GetWorld())->SwitchScreen("MenuScreen");
}

void UUDSettingsUserWidget::Save()
{
	AUDMenuHUD::Get(GetWorld())->SwitchScreen("MenuScreen");
}

void UUDSettingsUserWidget::WindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FUDWindowModeItem selected = *WindowModeItems.FindByPredicate(
		[&SelectedItem](const FUDWindowModeItem& item) { return item.ItemText == SelectedItem; }
	);
	ViewModel->SetSelectedWindowMode(selected);
}

void UUDSettingsUserWidget::ResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FUDResolutionItem selected = *ResolutionItems.FindByPredicate(
		[&SelectedItem](const FUDResolutionItem& item) { return item.ItemText == SelectedItem; }
	);
	ViewModel->SetSelectedResolution(selected);
}

void UUDSettingsUserWidget::LoadOptions()
{
	WindowModeComboBox->ClearOptions();
	WindowModeItems = ViewModel->CreateWindowModeOptions();
	for (FUDWindowModeItem& option : WindowModeItems)
	{		
		WindowModeComboBox->AddOption(option.ItemText);
	}

	ResolutionComboBox->ClearOptions();
	ResolutionItems = ViewModel->CreateResolutionOptions();
	for (FUDResolutionItem& option : ResolutionItems)
	{
		ResolutionComboBox->AddOption(option.ItemText);
	}
}