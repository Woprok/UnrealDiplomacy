// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDImperialThroneUserWidget.generated.h"

// Forward Declarations

class UButton;
class UTextBlock;
class UUDImperialThroneViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDImperialThroneUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDImperialThroneViewModel* model);
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
	TWeakObjectPtr<UTextBlock> DealCountTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> MessageCountTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ThroneTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> ThroneButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ThroneTextButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> DealButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> MessageButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDImperialThroneViewModel> ViewModel;
};