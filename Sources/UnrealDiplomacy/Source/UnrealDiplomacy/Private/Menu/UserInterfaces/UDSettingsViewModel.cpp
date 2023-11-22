// Copyright Miroslav Valach
// TODO Part of this should be simplified with properly binding comboboxes.
// Consequently the runtime update dependency is required to show saved value.
// This would be removed by properly applying binding for list of options and selected option.
// And finally it would be best if view did not depend that much on this view model.

#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/UDGlobalData.h"
#include "Core/UDGameInstance.h"
#include "Menu/UDMenuHUD.h"

#define LOCTEXT_NAMESPACE "Settings"

void UUDSettingsViewModel::Setup()
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
	CreateWindowModeOptions();
	CreateResolutionOptions();
}

void UUDSettingsViewModel::Refresh()
{
	// Load all settings.
	SetDefaultContent();
	// Propagate to UI, see TODO
	OnSettingsLoaded.Broadcast();
}

void UUDSettingsViewModel::CreateWindowModeOptions()
{
	WindowModes = {
		FUDWindowModeItem(EUDWindowModeType::Fullscreen, FText(LOCTEXT("Settings", "Fullscreen")).ToString()),
		FUDWindowModeItem(EUDWindowModeType::Borderless, FText(LOCTEXT("Settings", "Borderless")).ToString()),
		FUDWindowModeItem(EUDWindowModeType::Windowed, FText(LOCTEXT("Settings", "Windowed")).ToString()),
	};
}

void UUDSettingsViewModel::CreateResolutionOptions()
{
	Resolutions = {
		FUDResolutionItem(FIntPoint(1280, 720), FText(LOCTEXT("Settings", "1280x720")).ToString()),
		FUDResolutionItem(FIntPoint(1920, 1080), FText(LOCTEXT("Settings", "1920x1080")).ToString()),
		FUDResolutionItem(FIntPoint(2560, 1440), FText(LOCTEXT("Settings", "2560x1440")).ToString()),
	};
}

TArray<FString> UUDSettingsViewModel::GetWindowModeOptions() const
{
	TArray<FString> windowModes;
	for (const FUDWindowModeItem& windowMode : WindowModes)
	{
		windowModes.Add(windowMode.ItemText);
	}
	return windowModes;
}

TArray<FString> UUDSettingsViewModel::GetResolutionOptions() const
{
	TArray<FString> resolutions;
	for (const FUDResolutionItem& resolution : Resolutions)
	{
		resolutions.Add(resolution.ItemText);
	}
	return resolutions;
}

#undef LOCTEXT_NAMESPACE

void UUDSettingsViewModel::SetDefaultContent()
{
	FUDApplicationSettings settings = UUDGameInstance::Get(GetWorld())->LoadSettings();
	SelectedWindowMode = FindInWindowModes(settings.WindowMode, WindowModes);
	SelectedResolution = FindInResolutions(settings.Resolution, Resolutions);
}

void UUDSettingsViewModel::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSettingsViewModel: Back."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->MenuScreen);
}

void UUDSettingsViewModel::Save()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSettingsViewModel: Save."));
	SaveChanges();
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->MenuScreen);
}

void UUDSettingsViewModel::Credits()
{
	UE_LOG(LogTemp, Log, TEXT("UUDSettingsViewModel: Credits."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->CreditsScreen);
}

void UUDSettingsViewModel::SetSelectedWindowMode(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SelectedWindowMode = FindInWindowModes(SelectedItem, WindowModes);
}

FString UUDSettingsViewModel::GetSelectedWindowMode() const
{
	return SelectedWindowMode.ItemText;
}

void UUDSettingsViewModel::SetSelectedResolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SelectedResolution = FindInResolutions(SelectedItem, Resolutions);
}

FString UUDSettingsViewModel::GetSelectedResolution() const
{
	return SelectedResolution.ItemText;
}

void UUDSettingsViewModel::SaveChanges()
{
	FUDApplicationSettings newSettings = FUDApplicationSettings(
		SelectedResolution.ItemCode,
		SelectedWindowMode.ItemCode
	);
	UUDGameInstance::Get(GetWorld())->SaveSettings(newSettings);
}

FUDWindowModeItem UUDSettingsViewModel::FindInWindowModes(EUDWindowModeType searchedItem, const TArray<FUDWindowModeItem>& items) const
{
	FUDWindowModeItem selected = *items.FindByPredicate(
		[&searchedItem](const FUDWindowModeItem& item) { return item.ItemCode == searchedItem; }
	);
	return selected;
}

FUDWindowModeItem UUDSettingsViewModel::FindInWindowModes(FString searchedItem, const TArray<FUDWindowModeItem>& items) const
{
	FUDWindowModeItem selected = *items.FindByPredicate(
		[&searchedItem](const FUDWindowModeItem& item) { return item.ItemText == searchedItem; }
	);
	return selected;
}

FUDResolutionItem UUDSettingsViewModel::FindInResolutions(FIntPoint searchedItem, const TArray<FUDResolutionItem>& items) const
{
	FUDResolutionItem selected = *items.FindByPredicate(
		[&searchedItem](const FUDResolutionItem& item) { return item.ItemCode == searchedItem; }
	);
	return selected;
}

FUDResolutionItem UUDSettingsViewModel::FindInResolutions(FString searchedItem, const TArray<FUDResolutionItem>& items) const
{
	FUDResolutionItem selected = *items.FindByPredicate(
		[&searchedItem](const FUDResolutionItem& item) { return item.ItemText == searchedItem; }
	);
	return selected;
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