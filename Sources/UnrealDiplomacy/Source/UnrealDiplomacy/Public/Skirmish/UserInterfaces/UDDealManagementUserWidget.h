// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDDealManagementUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDDealManagementViewModel;
class UUDDealItemViewModel;
class UUDDealItemUserWidget;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealManagementUserWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDealManagementViewModel* model);
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
	/** Callback to set data from view model. */
	void SetActiveItemInstance(const TObjectPtr<UUDDealItemViewModel>& dealItemViewModel);
	/** Callback to set data from view model. */
	void SetHistoryItemInstance(const TObjectPtr<UUDDealItemViewModel>& dealItemViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> DealManagementTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> DealCountTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CloseTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> FirstTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> PreviousTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NextTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> LastTextWidget;	
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CreateTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ActiveTextWidget;	
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> HistoryTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> CreateButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ActiveButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> HistoryButtonWidget;
	
	UPROPERTY()
	TWeakObjectPtr<UButton> CloseButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> FirstButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> PreviousButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> NextButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> LastButtonWidget;

	UPROPERTY()
	TWeakObjectPtr<UUDDealItemUserWidget> ActiveDealItemInstanceWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDDealItemUserWidget> HistoryDealItemInstanceWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealManagementViewModel> ViewModel;
};