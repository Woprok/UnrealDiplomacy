// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDSettingsUserWidget.h"
#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"

void UUDSettingsUserWidget::BindViewModel(TObjectPtr<UUDViewModel> viewModel)
{
	UUDSettingsViewModel* settingsViewModel = CastChecked<UUDSettingsViewModel>(viewModel.Get());
	ViewModel = settingsViewModel;
	SetBlueprintViewModel(ViewModel.Get());
}

void UUDSettingsUserWidget::BindDelegates()
{
	// Bind view to updates.
	ViewModel->OnSettingsLoaded.AddUniqueDynamic(this, &UUDSettingsUserWidget::SetOptions);
	// Bind viewmodel to widgets.
	BackButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDSettingsViewModel::Back);
	SaveButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDSettingsViewModel::Save);
	CreditsButtonWidget->OnClicked.AddUniqueDynamic(ViewModel.Get(), &UUDSettingsViewModel::Credits);
	WindowModeComboBoxWidget->OnSelectionChanged.AddUniqueDynamic(ViewModel.Get(), &UUDSettingsViewModel::SetSelectedWindowMode);
	ResolutionComboBoxWidget->OnSelectionChanged.AddUniqueDynamic(ViewModel.Get(), &UUDSettingsViewModel::SetSelectedResolution);
}

void UUDSettingsUserWidget::BindWidgets()
{
	BackButtonWidget = GetWidget<UButton>(TEXT("BackButton"));
	SaveButtonWidget = GetWidget<UButton>(TEXT("SaveButton"));
	CreditsButtonWidget = GetWidget<UButton>(TEXT("CreditsButton"));
	WindowModeComboBoxWidget = GetWidget<UComboBoxString>(TEXT("WindowModeComboBox"));
	ResolutionComboBoxWidget = GetWidget<UComboBoxString>(TEXT("ResolutionComboBox"));
}

void UUDSettingsUserWidget::SetOptions()
{
	SetWindowModeFromSource();
	SetResolutionFromSource();
}

void UUDSettingsUserWidget::SetWindowModeFromSource()
{
	// Clear and Set
	WindowModeComboBoxWidget->ClearOptions();
	WindowModeItems = ViewModel->GetWindowModeOptions();
	for (FString& option : WindowModeItems)
	{
		WindowModeComboBoxWidget->AddOption(option);
	}

	FString selectedWindowMode = ViewModel->GetSelectedWindowMode();
	WindowModeComboBoxWidget->SetSelectedOption(selectedWindowMode);
}

void UUDSettingsUserWidget::SetResolutionFromSource()
{
	// Clear and Set
	ResolutionComboBoxWidget->ClearOptions();
	ResolutionItems = ViewModel->GetResolutionOptions();
	for (FString& option : ResolutionItems)
	{
		ResolutionComboBoxWidget->AddOption(option);
	}

	FString selectedResolution = ViewModel->GetSelectedResolution();
	ResolutionComboBoxWidget->SetSelectedOption(selectedResolution);
}