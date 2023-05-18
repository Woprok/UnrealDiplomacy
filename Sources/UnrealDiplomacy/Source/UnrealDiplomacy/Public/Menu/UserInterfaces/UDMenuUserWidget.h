// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDMenuUserWidget.generated.h"

// Fordward Declarations

class UTextBlock;
class UButton;
class UUDMenuViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDMenuUserWidget : public UUDUserWidget
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
	void SetBlueprintViewModel(UUDMenuViewModel* model);
protected:
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindDelegates() override;
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> GameTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CreateGameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> JoinGameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> SettingsTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> QuitTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CreateGameButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> JoinGameButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> SettingsButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> QuitButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDMenuViewModel> ViewModel;
private:
	// Button Functions
	UFUNCTION()
	void CreateGame();
	UFUNCTION()
	void JoinGame();
	UFUNCTION()
	void Settings();
	UFUNCTION()
	void Quit();
};