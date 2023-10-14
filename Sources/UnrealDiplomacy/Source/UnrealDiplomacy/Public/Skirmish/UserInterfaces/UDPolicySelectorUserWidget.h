// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDPolicySelectorUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UListView;
class UUDPolicySelectorViewModel;
class UUDPolicySelectItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDPolicySelectorUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDPolicySelectorViewModel* model);
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
	void SetPolicySelectItemSourceCollection(const TArray<TObjectPtr<UUDPolicySelectItemViewModel>>& policySelectItemViewModels);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> PolicyTitleTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UListView> PolicyItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDPolicySelectorViewModel> ViewModel;
};