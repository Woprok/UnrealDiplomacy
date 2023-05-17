// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDSettingsViewModel.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/UDGlobalData.h"
#include "Kismet/GameplayStatics.h"
#include "Core/UDGameInstance.h"

#define LOCTEXT_NAMESPACE "Settings"

void UUDSettingsViewModel::Update()
{
	FString windowMode = FText(LOCTEXT("Settings", "Window Mode")).ToString();
	SetWindowModeText(windowMode);
	FString resolution = FText(LOCTEXT("Settings", "Resolution")).ToString();
	SetResolutionText(resolution);
	FString credits = FText(LOCTEXT("Settings", "Credits")).ToString();
	SetCreditsText(credits);
	FString back = FText(LOCTEXT("Settings", "Back")).ToString();
	SetBackText(back);
	FString save = FText(LOCTEXT("Settings", "Save")).ToString();
	SetSaveText(save);
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
		FUDResolutionItem(1280, 0720, FText(LOCTEXT("Settings", "1280x720")).ToString()),
		FUDResolutionItem(1920, 1080, FText(LOCTEXT("Settings", "1920x1080")).ToString()),
		FUDResolutionItem(2560, 1440, FText(LOCTEXT("Settings", "2560x1440")).ToString()),
	};
}

#undef LOCTEXT_NAMESPACE

void UUDSettingsViewModel::SaveChanges()
{
	FUDApplicationSettings newSettings = FUDApplicationSettings(


	);
	CastChecked<UUDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SaveSettings(newSettings);
}

void UUDSettingsViewModel::SetSelectedWindowMode(FUDWindowModeItem selectedWindowMode)
{

}

void UUDSettingsViewModel::SetSelectedResolution(FUDResolutionItem selectedResolution)
{

}

void UUDSettingsViewModel::SetWindowModeText(FString newWindowModeText)
{
	UE_MVVM_SET_PROPERTY_VALUE(WindowModeText, newWindowModeText);
}

FString UUDSettingsViewModel::GetWindowModeText() const
{
	return WindowModeText;
}

void UUDSettingsViewModel::SetResolutionText(FString newResolutionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ResolutionText, newResolutionText);
}

FString UUDSettingsViewModel::GetResolutionText() const
{
	return ResolutionText;
}

void UUDSettingsViewModel::SetCreditsText(FString newCreditsText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreditsText, newCreditsText);
}

FString UUDSettingsViewModel::GetCreditsText() const
{
	return CreditsText;
}

void UUDSettingsViewModel::SetBackText(FString newBackText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackText, newBackText);
}

FString UUDSettingsViewModel::GetBackText() const
{
	return BackText;
}

void UUDSettingsViewModel::SetSaveText(FString newSaveText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SaveText, newSaveText);
}

FString UUDSettingsViewModel::GetSaveText() const
{
	return SaveText;
}