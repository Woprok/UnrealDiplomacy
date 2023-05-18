// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDCreditsUserWidget.generated.h"

// Forward Declaratiosn

class UTextBlock;
class UButton;
class UUDCreditsViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDCreditsUserWidget : public UUDUserWidget
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
	void SetBlueprintViewModel(UUDCreditsViewModel* model);
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
	TWeakObjectPtr<UTextBlock> CreditsTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ContentTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> BackTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> BackButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDCreditsViewModel> ViewModel;
private:
	// Button Functions
	UFUNCTION()
	void Back();
};