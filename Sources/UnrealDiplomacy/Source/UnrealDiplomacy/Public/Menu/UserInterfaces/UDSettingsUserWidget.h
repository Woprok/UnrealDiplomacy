// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDSettingsUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UComboBoxString;
class UUDSettingsViewModel;
struct FUDWindowModeItem;
struct FUDResolutionItem;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDSettingsUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables to define view model used by this widet for delegates.
	 */
	virtual void SetViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDSettingsViewModel* model);
protected:
	/**
	 * Automatically invoked by native construct.
	 */
	virtual void BindDelegates() override;
	/**
	 * Loads data from view model.
	 */
	UFUNCTION()
	void LoadOptions();
protected:
	// Bindings
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SettingsTitleText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> BackText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SaveButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SaveText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> CreditsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CreditsText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> WindowModeComboBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> WindowModeText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UComboBoxString> ResolutionComboBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ResolutionText;
	// ViewModel
	UPROPERTY()
	TObjectPtr<UUDSettingsViewModel> ViewModel;
private:
	// Button Functions
	UFUNCTION()
	void Back();
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Credits();
	// ComboBox Functions
	UFUNCTION()
	void WindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void ResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void LoadWindowMode(); 
	UFUNCTION()
	void LoadResolution();
private:
	// ComboBox Arrays
	TArray<FUDWindowModeItem> WindowModeItems;
	TArray<FUDResolutionItem> ResolutionItems;
};