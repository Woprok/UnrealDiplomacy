// Copyright Miroslav Valach

#include "Core/UDGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/UDGlobalData.h"

TObjectPtr<UUDGameInstance> UUDGameInstance::Get(TObjectPtr<UWorld> world)
{
    TObjectPtr<UGameInstance> gameInstance = UGameplayStatics::GetGameInstance(world);
    check(gameInstance != nullptr);
    return CastChecked<UUDGameInstance>(gameInstance);
}

FUDApplicationSettings UUDGameInstance::LoadSettings()
{
    TObjectPtr<UGameUserSettings> globalSettings = GetGameUserSettings();
    if (globalSettings != nullptr)
        return FUDApplicationSettings::GetDefault();

    // Assuming that settings are present...
    EWindowMode::Type type = globalSettings->GetFullscreenMode();
    EUDWindowModeType windowMode = UUDApplicationConverters::ToWindowModeType(type);
    FIntPoint resolution = globalSettings->GetScreenResolution();
    // Return loaded settings wrapped.
    FUDApplicationSettings loadedSettings = FUDApplicationSettings(resolution, windowMode);
    return loadedSettings;
}

void UUDGameInstance::SaveSettings(FUDApplicationSettings newSettings)
{
    TObjectPtr<UGameUserSettings> globalSettings = GetGameUserSettings();

    FIntPoint newResolution = newSettings.Resolution;
    EWindowMode::Type newWindowMode = UUDApplicationConverters::FromWindowModeType(newSettings.WindowMode);
    // Set all defined settings.
    globalSettings->SetScreenResolution(newResolution);
    globalSettings->SetFullscreenMode(newWindowMode);
    // Apply & Save.
    globalSettings->ApplyNonResolutionSettings();
    globalSettings->ApplyResolutionSettings(false);
    globalSettings->SaveSettings();
    globalSettings->ApplySettings(false);
}

void UUDGameInstance::Quit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UUDGameInstance::OpenMenuLevel(FString options)
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MenuLevel, true, options);
}

void UUDGameInstance::OpenSkirmishLevel(FString options)
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), SkirmishLevel, true, options);
}

FString UUDGameInstance::GetSkirmishLevelName()
{
    return SkirmishLevel.GetAssetName();
}

TObjectPtr<UGameUserSettings> UUDGameInstance::GetGameUserSettings()
{
    if (GEngine != nullptr)
    {
        return GEngine->GameUserSettings;
    }
    return nullptr;
}