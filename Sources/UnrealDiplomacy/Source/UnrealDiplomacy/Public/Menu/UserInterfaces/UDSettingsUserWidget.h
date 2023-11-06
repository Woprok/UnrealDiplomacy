// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDSettingsUserWidget.generated.h"

// Forward Declarations

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
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDSettingsViewModel* model);
protected:
	/**
	 * Enables to define view model used by this widet for delegates.
	 */
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindDelegates() override;
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindWidgets() override;
	/**
	 * Updates all data that are depending on view model.
	 */
	UFUNCTION()
	void SetOptions();
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> BackButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> SaveButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CreditsButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UComboBoxString> WindowModeComboBoxWidget;
	UPROPERTY()
	TWeakObjectPtr<UComboBoxString> ResolutionComboBoxWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDSettingsViewModel> ViewModel;
private:
	/**
	 * Retrives data from view model and set this widget to align with them.
	 */
	UFUNCTION()
	void SetWindowModeFromSource();
	/**
	 * Retrives data from view model and set this widget to align with them.
	 */
	UFUNCTION()
	void SetResolutionFromSource();
private:
	// ComboBox Arrays
	TArray<FString> WindowModeItems;
	TArray<FString> ResolutionItems;
};