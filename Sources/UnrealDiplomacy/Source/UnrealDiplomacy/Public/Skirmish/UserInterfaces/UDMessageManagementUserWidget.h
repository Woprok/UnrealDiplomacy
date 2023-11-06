// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDMessageManagementUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDMessageManagementViewModel;
class UUDMessageItemViewModel;
class UUDMessageItemUserWidget;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDMessageManagementUserWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDMessageManagementViewModel* model);
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
	void SetMessageItemSourceInstance(const TObjectPtr<UUDMessageItemViewModel>& messageItemViewModel);
protected:
	// Bindings
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
	TWeakObjectPtr<UUDMessageItemUserWidget> MessageItemViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDMessageManagementViewModel> ViewModel;
};