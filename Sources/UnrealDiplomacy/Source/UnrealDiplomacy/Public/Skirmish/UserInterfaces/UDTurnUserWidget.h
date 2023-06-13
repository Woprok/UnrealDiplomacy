// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDTurnUserWidget.generated.h"

// Forward Declarations

class UUDTurnViewModel;
class UButton;
class UTextBlock;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDTurnUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDTurnViewModel* model);
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
	TWeakObjectPtr<UTextBlock> RegentTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> RemainingTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> FinishTurnTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> FinishTurnButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDTurnViewModel> ViewModel;
};