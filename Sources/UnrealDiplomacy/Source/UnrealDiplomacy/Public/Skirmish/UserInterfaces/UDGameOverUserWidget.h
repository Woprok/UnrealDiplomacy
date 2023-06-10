// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDGameOverUserWidget.generated.h"

// Forward Declarations

class UUDGameOverViewModel;
class UButton;
class UTextBlock;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDGameOverUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDGameOverViewModel* model);
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
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> GameOverTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> WinnerFactionTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ReturnToMenuTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> ReturnButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDGameOverViewModel> ViewModel;
};