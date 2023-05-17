// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDSettingsViewModel.generated.h"

// Forward Declarations

enum class EUDWindowModeType : uint8;

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
	 * Creates new list of all supported window options.
	 */
	TArray<FUDWindowModeItem> CreateWindowModeOptions() const;
	/**
	 * Creates new list of all supported resolution options.
	 */
	TArray<FUDResolutionItem> CreateResolutionOptions() const;
	/**
	 * Applies changes that are present in the view model.
	 */
	void SaveChanges();
public:
	// MVVM 
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString WindowModeText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString ResolutionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString SaveText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CreditsText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString SettingsTitleText;
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	EUDWindowModeType GetSelectedWindowMode() const;
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	void SetSelectedWindowMode(const FUDWindowModeItem& selectedWindowMode);
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	FIntPoint GetSelectedResolution() const;
	/**
	 * Binding for non-primitive invoked by widget.
	 */
	void SetSelectedResolution(const FUDResolutionItem& selectedResolution);
protected:
	/**
	 * Invoked for each update.
	 */
	virtual void Update() override;
	/**
	 * Loads settings from game instance.
	 */
	void RetrieveSettings();
private:
	// MVVM Setters & Getters
	void SetWindowModeText(FString newWindowModeText);
	FString GetWindowModeText() const;
	void SetResolutionText(FString newResolutionText);
	FString GetResolutionText() const;
	void SetBackText(FString newBackText);
	FString GetBackText() const;
	void SetSaveText(FString newSaveText);
	FString GetSaveText() const;
	void SetCreditsText(FString newCreditsText);
	FString GetCreditsText() const;
	void SetSettingsTitleText(FString newSettingsTitleText);
	FString GetSettingsTitleText() const;
private:
	// Fields
	EUDWindowModeType SelectedWindowMode;
	FIntPoint SelectedResolution;
};