// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDFactionParameterUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDFactionParameterViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDFactionParameterUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDFactionParameterViewModel* model);
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
	TWeakObjectPtr<UTextBlock> FactionTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> PreviousTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NextTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> PreviousButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> NextButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDFactionParameterViewModel> ViewModel;
};