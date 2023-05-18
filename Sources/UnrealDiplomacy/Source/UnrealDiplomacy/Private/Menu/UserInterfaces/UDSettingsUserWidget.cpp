// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDSettingsUserWidget.h"
#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "Components/TextBlock.h"
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

void UUDSettingsUserWidget::BindWidgets()
{
	SettingsTitleTextWidget = GetWidget<UTextBlock>(TEXT("SettingsTitleText"));
	BackTextWidget = GetWidget<UTextBlock>(TEXT("BackText"));
	SaveTextWidget = GetWidget<UTextBlock>(TEXT("SaveText"));
	CreditsTextWidget = GetWidget<UTextBlock>(TEXT("CreditsText"));
	WindowModeTextWidget = GetWidget<UTextBlock>(TEXT("WindowModeText"));
	ResolutionTextWidget = GetWidget<UTextBlock>(TEXT("ResolutionText"));
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
	SaveButtonWidget = GetWidget<UButton>(TEXT("SaveButton"));
	CreditsButtonWidget = GetWidget<UButton>(TEXT("CreditsButton"));
	WindowModeComboBoxWidget = GetWidget<UComboBoxString>(TEXT("WindowModeComboBox"));
	ResolutionComboBoxWidget = GetWidget<UComboBoxString>(TEXT("ResolutionComboBox"));
}

void UUDSettingsUserWidget::BindDelegates()
{
	BackButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Back);
	SaveButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Save);
	CreditsButtonWidget->OnClicked.AddUniqueDynamic(this, &UUDSettingsUserWidget::Credits);
	WindowModeComboBoxWidget->OnSelectionChanged.AddUniqueDynamic(this, &UUDSettingsUserWidget::WindowModeChanged);
	ResolutionComboBoxWidget->OnSelectionChanged.AddUniqueDynamic(this, &UUDSettingsUserWidget::ResolutionChanged);
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
	WindowModeComboBoxWidget->ClearOptions();
	WindowModeItems = ViewModel->CreateWindowModeOptions();
	for (FUDWindowModeItem& option : WindowModeItems)
	{
		WindowModeComboBoxWidget->AddOption(option.ItemText);
	}
	EUDWindowModeType selectedWindowMode = ViewModel->GetSelectedWindowMode();

	FUDWindowModeItem selectedOption = *WindowModeItems.FindByPredicate(
		[&selectedWindowMode](const FUDWindowModeItem& item) { return item.ItemCode == selectedWindowMode; }
	);
	WindowModeComboBoxWidget->SetSelectedOption(selectedOption.ItemText);
}

void UUDSettingsUserWidget::LoadResolution()
{
	ResolutionComboBoxWidget->ClearOptions();
	ResolutionItems = ViewModel->CreateResolutionOptions();
	for (FUDResolutionItem& option : ResolutionItems)
	{
		ResolutionComboBoxWidget->AddOption(option.ItemText);
	}

	FIntPoint selectedResolution = ViewModel->GetSelectedResolution();

	FUDResolutionItem selectedOption = *ResolutionItems.FindByPredicate(
		[&selectedResolution](const FUDResolutionItem& item) { return item.ItemCode == selectedResolution; }
	);
	ResolutionComboBoxWidget->SetSelectedOption(selectedOption.ItemText);
}