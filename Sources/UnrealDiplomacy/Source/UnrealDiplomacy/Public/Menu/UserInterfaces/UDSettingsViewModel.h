// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDSettingsViewModel.generated.h"

// Forward Declarations

enum class EUDWindowModeType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDSettingsLoaded);

/**
 * Wrapper around items that will be in view.
 */
USTRUCT(BlueprintType)
struct FUDWindowModeItem
{
	GENERATED_BODY()
public:
	FUDWindowModeItem() { }
	FUDWindowModeItem(EUDWindowModeType itemCode, FString itemText)
		: ItemCode(itemCode), ItemText(itemText) { }
	UPROPERTY(BlueprintReadOnly)
	EUDWindowModeType ItemCode;
	UPROPERTY(BlueprintReadOnly)
	FString ItemText;
};

/**
 * Wrapper around items that will be in view.
 */
USTRUCT(BlueprintType)
struct FUDResolutionItem
{
	GENERATED_BODY()
public:
	FUDResolutionItem() { }
	FUDResolutionItem(FIntPoint itemCode, FString itemText)
		: ItemCode(itemCode), ItemText(itemText) { }
	UPROPERTY(BlueprintReadOnly)
	FIntPoint ItemCode;
	UPROPERTY(BlueprintReadOnly)
	FString ItemText;
};

/**
 * ViewModel for updating settings.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSettingsViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Gets new list from all supported window options.
	 */
	TArray<FString> GetWindowModeOptions() const;
	/**
	 * Gets new list from all supported resolution options.
	 */
	TArray<FString> GetResolutionOptions() const;
public:
	// Button Functions
	UFUNCTION()
	void Back();
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Credits();
	// MVVM 
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText WindowModeText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ResolutionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SaveText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreditsText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SettingsTitleText;
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	FString GetSelectedWindowMode() const;
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	UFUNCTION()
	void SetSelectedWindowMode(FString SelectedItem, ESelectInfo::Type SelectionType);
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	FString GetSelectedResolution() const;
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	UFUNCTION()
	void SetSelectedResolution(FString SelectedItem, ESelectInfo::Type SelectionType);
	/**
	 * Subscribeable event that is invoked after the update.
	 * Useful for views to update their data to fit current view model.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDSettingsLoaded OnSettingsLoaded;
protected:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	/**
	 * Applies changes that are present in the view model to the settings in game instance.
	 */
	void SaveChanges();
	/**
	 * Loads current settings from game instance.
	 */
	void RetrieveSettings();
	/**
	 * Creates new list of all supported window options.
	 */
	void CreateWindowModeOptions();
	/**
	 * Creates new list of all supported resolution options.
	 */
	void CreateResolutionOptions();
	FUDWindowModeItem FindInWindowModes(EUDWindowModeType searchedItem, const TArray<FUDWindowModeItem>& items) const;
	FUDWindowModeItem FindInWindowModes(FString searchedItem, const TArray<FUDWindowModeItem>& items) const;
	FUDResolutionItem FindInResolutions(FIntPoint searchedItem, const TArray<FUDResolutionItem>& items) const;
	FUDResolutionItem FindInResolutions(FString searchedItem, const TArray<FUDResolutionItem>& items) const;
private:
	// MVVM Setters & Getters
	void SetWindowModeText(FText newWindowModeText);
	FText GetWindowModeText() const;
	void SetResolutionText(FText newResolutionText);
	FText GetResolutionText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
	void SetSaveText(FText newSaveText);
	FText GetSaveText() const;
	void SetCreditsText(FText newCreditsText);
	FText GetCreditsText() const;
	void SetSettingsTitleText(FText newSettingsTitleText);
	FText GetSettingsTitleText() const;
private:
	// Fields
	TArray<FUDWindowModeItem> WindowModes;
	TArray<FUDResolutionItem> Resolutions;
	FUDWindowModeItem SelectedWindowMode;
	FUDResolutionItem SelectedResolution;
};