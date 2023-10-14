// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDFactionManagementUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UListView;
class UUDFactionManagementViewModel;
class UUDFactionInteractionViewModel;
class UUDModifierItemViewModel;
class UUDPolicySelectorUserWidget;
class UUDPolicySelectorViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDFactionManagementUserWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDFactionManagementViewModel* model);
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
	void SetFactionInteractionSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionInteractionViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetFactionOfferSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionOfferViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetFactionRequestSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionRequestViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetFactionDemandSourceCollection(const TArray<TObjectPtr<UUDFactionInteractionViewModel>>& factionDemandViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetModifierItemSourceCollection(const TArray<TObjectPtr<UUDModifierItemViewModel>>& modifierItemViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetPolicySelectorSourceInstance(const TObjectPtr<UUDPolicySelectorViewModel>& policySelectorViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> FactionManagementTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> FactionNameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CloseTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CloseButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> ModifierItemListWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> InteractionTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> OfferTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> RequestTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> DemandTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> FactionInteractionListWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> FactionOfferListWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> FactionRequestListWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> FactionDemandListWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDPolicySelectorUserWidget> PolicySelectorViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDFactionManagementViewModel> ViewModel;
};