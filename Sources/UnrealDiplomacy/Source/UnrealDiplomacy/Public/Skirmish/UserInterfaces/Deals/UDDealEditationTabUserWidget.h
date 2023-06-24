// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDDealItemUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UCheckBox;
class UUDDealItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealItemUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDealItemViewModel* model);
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
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealItemViewModel> ViewModel;
};