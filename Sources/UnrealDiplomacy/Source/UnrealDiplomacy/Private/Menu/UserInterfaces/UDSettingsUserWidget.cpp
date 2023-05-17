// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDSettingsUserWidget.h"
#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Menu/UDMenuHUD.h"

void UUDSettingsUserWidget::SetViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDSettingsViewModel* settingsViewModel = CastChecked<UUDSettingsViewModel>(viewModel.Get());
	ViewModel = settingsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
	// Bind to updates.
	ViewModel->OnUpdateFinishing.AddUniqueDynamic(this, &UUDSettingsUserWidget::LoadOptions);
}

void UUDSettingsUserWidget::BindDelegates()
{
	BackButton->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Back);
	SaveButton->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Save);
	WindowModeComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UUDSettingsUserWidget::WindowModeChanged);
	ResolutionComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UUDSettingsUserWidget::ResolutionChanged);
}

void UUDSettingsUserWidget::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSettingsUserWidget: Back."));
	AUDMenuHUD::Get(GetWorld())->SwitchScreen("MenuScreen");
}

void UUDSettingsUserWidget::Save()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSettingsUserWidget: Save."));
	AUDMenuHUD::Get(GetWorld())->SwitchScreen("MenuScreen");
}

void UUDSettingsUserWidget::Credits()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSettingsUserWidget: Credits."));
	AUDMenuHUD::Get(GetWorld())->SwitchScreen("Credits");
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
	LoadWindowMode();
	LoadResolution();
}

void UUDSettingsUserWidget::LoadWindowMode()
{
	WindowModeComboBox->ClearOptions();
	WindowModeItems = ViewModel->CreateWindowModeOptions();
	for (FUDWindowModeItem& option : WindowModeItems)
	{
		WindowModeComboBox->AddOption(option.ItemText);
	}
	EUDWindowModeType selectedWindowMode = ViewModel->GetSelectedWindowMode();

	FUDWindowModeItem selectedOption = *WindowModeItems.FindByPredicate(
		[&selectedWindowMode](const FUDWindowModeItem& item) { return item.ItemCode == selectedWindowMode; }
	);
	WindowModeComboBox->SetSelectedOption(selectedOption.ItemText);
}

void UUDSettingsUserWidget::LoadResolution()
{
	ResolutionComboBox->ClearOptions();
	ResolutionItems = ViewModel->CreateResolutionOptions();
	for (FUDResolutionItem& option : ResolutionItems)
	{
		ResolutionComboBox->AddOption(option.ItemText);
	}

	FIntPoint selectedResolution = ViewModel->GetSelectedResolution();

	FUDResolutionItem selectedOption = *ResolutionItems.FindByPredicate(
		[&selectedResolution](const FUDResolutionItem& item) { return item.ItemCode == selectedResolution; }
	);
	ResolutionComboBox->SetSelectedOption(selectedOption.ItemText);
}