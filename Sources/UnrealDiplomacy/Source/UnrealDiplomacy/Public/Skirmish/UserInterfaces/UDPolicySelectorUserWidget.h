// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDPolicySelectorUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UUDListView;
class UUDPolicySelectorViewModel;
class UUDPolicySelectItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
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
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> PolicyTitleTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UUDListView> PolicyItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDPolicySelectorViewModel> ViewModel;
};