// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDDealItemUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDDealItemViewModel;
class UUDDealGeneralTabUserWidget;
class UUDDealGeneralTabViewModel;
class UUDDealEditationTabUserWidget;
class UUDDealEditationTabViewModel;
class UUDDealExecutionTabUserWidget;
class UUDDealExecutionTabViewModel;

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
	/**
	 * Callback to set data from view model.
	 */
	void SetGeneralTabSourceInstance(const TObjectPtr<UUDDealGeneralTabViewModel>& tabViewModel);
	/**
	 * Callback to set data from view model.
	 */
	void SetEditationTabSourceInstance(const TObjectPtr<UUDDealEditationTabViewModel>& tabViewModel);
	/**
	 * Callback to set data from view model.
	 */
	void SetExecutionTabSourceInstance(const TObjectPtr<UUDDealExecutionTabViewModel>& tabViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> DealNameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> GeneralTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> EditationTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ExecutableTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> GeneralButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> EditationButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ExecutionButtonWidget;

	UPROPERTY()
	TWeakObjectPtr<UUDDealGeneralTabUserWidget> DealGeneralTabViewWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDDealEditationTabUserWidget> DealEditationTabViewWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDDealExecutionTabUserWidget> DealExecutionTabViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealItemViewModel> ViewModel;
};