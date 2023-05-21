// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/UDGlobalData.h"
#include "Core/UDGameInstance.h"

#define LOCTEXT_NAMESPACE "Settings"

void UUDSettingsViewModel::Update()
{
	FText settingsTitle = FText(LOCTEXT("Settings", "Settings"));
	SetSettingsTitleText(settingsTitle);
	FText windowMode = FText(LOCTEXT("Settings", "Window Mode"));
	SetWindowModeText(windowMode);
	FText resolution = FText(LOCTEXT("Settings", "Resolution"));
	SetResolutionText(resolution);
	FText back = FText(LOCTEXT("Settings", "Back"));
	SetBackText(back);
	FText save = FText(LOCTEXT("Settings", "Save"));
	SetSaveText(save);
	FText credits = FText(LOCTEXT("Settings", "Credits"));
	SetCreditsText(credits);
	// Load all settings.
	RetrieveSettings();
}

TArray<FUDWindowModeItem> UUDSettingsViewModel::CreateWindowModeOptions() const
{
	return {
		FUDWindowModeItem(EUDWindowModeType::Fullscreen, FText(LOCTEXT("Settings", "Fullscreen")).ToString()),
		FUDWindowModeItem(EUDWindowModeType::Borderless, FText(LOCTEXT("Settings", "Borderless")).ToString()),
		FUDWindowModeItem(EUDWindowModeType::Windowed, FText(LOCTEXT("Settings", "Windowed")).ToString()),
	};
}

TArray<FUDResolutionItem> UUDSettingsViewModel::CreateResolutionOptions() const
{
	return {
		FUDResolutionItem(FIntPoint(1280, 720), FText(LOCTEXT("Settings", "1280x720")).ToString()),
		FUDResolutionItem(FIntPoint(1920, 1080), FText(LOCTEXT("Settings", "1920x1080")).ToString()),
		FUDResolutionItem(FIntPoint(2560, 1440), FText(LOCTEXT("Settings", "2560x1440")).ToString()),
	};
}

#undef LOCTEXT_NAMESPACE

void UUDSettingsViewModel::SaveChanges()
{
	FUDApplicationSettings newSettings = FUDApplicationSettings(
		SelectedResolution,
		SelectedWindowMode
	);
	UUDGameInstance::Get(GetWorld())->SaveSettings(newSettings);
}

void UUDSettingsViewModel::RetrieveSettings()
{
	FUDApplicationSettings settings = UUDGameInstance::Get(GetWorld())->LoadSettings();
	SelectedResolution = settings.Resolution;
	SelectedWindowMode = settings.WindowMode;
}

EUDWindowModeType UUDSettingsViewModel::GetSelectedWindowMode() const
{
	return SelectedWindowMode;
}

void UUDSettingsViewModel::SetSelectedWindowMode(const FUDWindowModeItem& selectedWindowMode)
{
	SelectedWindowMode = selectedWindowMode.ItemCode;
}

FIntPoint UUDSettingsViewModel::GetSelectedResolution() const
{
	return SelectedResolution;
}

void UUDSettingsViewModel::SetSelectedResolution(const FUDResolutionItem& selectedResolution)
{
	SelectedResolution = selectedResolution.ItemCode;
}

void UUDSettingsViewModel::SetWindowModeText(FText newWindowModeText)
{
	UE_MVVM_SET_PROPERTY_VALUE(WindowModeText, newWindowModeText);
}

FText UUDSettingsViewModel::GetWindowModeText() const
{
	return WindowModeText;
}

void UUDSettingsViewModel::SetResolutionText(FText newResolutionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ResolutionText, newResolutionText);
}

FText UUDSettingsViewModel::GetResolutionText() const
{
	return ResolutionText;
}

void UUDSettingsViewModel::SetBackText(FText newBackText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackText, newBackText);
}

FText UUDSettingsViewModel::GetBackText() const
{
	return BackText;
}

void UUDSettingsViewModel::SetSaveText(FText newSaveText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SaveText, newSaveText);
}

FText UUDSettingsViewModel::GetSaveText() const
{
	return SaveText;
}

void UUDSettingsViewModel::SetCreditsText(FText newCreditsText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreditsText, newCreditsText);
}

FText UUDSettingsViewModel::GetCreditsText() const
{
	return CreditsText;
}

void UUDSettingsViewModel::SetSettingsTitleText(FText newSettingsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SettingsTitleText, newSettingsTitleText);
}

FText UUDSettingsViewModel::GetSettingsTitleText() const
{
	return SettingsTitleText;
}