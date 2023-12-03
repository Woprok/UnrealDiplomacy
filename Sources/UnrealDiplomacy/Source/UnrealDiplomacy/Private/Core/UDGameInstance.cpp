// Copyright Miroslav Valach

#include "Core/UDGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionManager.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDResourceManager.h"

UUDGameInstance::UUDGameInstance()
{
    ActionManagerType = UUDActionManager::StaticClass();
    ModifierManagerType = UUDModifierManager::StaticClass();
    ResourceManagerType = UUDResourceManager::StaticClass();
    UE_LOG(LogTemp, Log, TEXT("UUDGameInstance: Defined static classes for UUDGameInstance."));
}

TObjectPtr<UUDGameInstance> UUDGameInstance::Get(TObjectPtr<UWorld> world)
{
    TObjectPtr<UGameInstance> gameInstance = UGameplayStatics::GetGameInstance(world);
    //    TObjectPtr<UGameInstance> gameInstance = UGameplayStatics::GetGameInstance(GEngine->GetWorld());
    check(gameInstance != nullptr);
    return CastChecked<UUDGameInstance>(gameInstance);
}

void UUDGameInstance::Init()
{
    // Call parent to do it's thing.
    Super::Init();
    // Setup instances
    ActionManager = NewObject<UUDActionManager>(this, ActionManagerType);
    ModifierManager = NewObject<UUDModifierManager>(this, ModifierManagerType);
    ResourceManager = NewObject<UUDResourceManager>(this, ResourceManagerType);
    // First we initialize these that do not require dependencies.
    ModifierManager->Initialize();
    ResourceManager->Initialize();
    // Finally we finish initialization on action manager.
    ActionManager->SetResourceManager(ResourceManager);
    ActionManager->SetModifierManager(ModifierManager);
    // Requires other managers to be present (as actions work over all managers).
    ActionManager->Initialize();
}

TWeakObjectPtr<UUDActionManager> UUDGameInstance::GetActionManager()
{
    return ActionManager;
}

TWeakObjectPtr<UUDModifierManager> UUDGameInstance::GetModifierManager()
{
    return ModifierManager;
}

TWeakObjectPtr<UUDResourceManager> UUDGameInstance::GetResourceManager()
{
    return ResourceManager;
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