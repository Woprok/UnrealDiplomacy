// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDActionParameterUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDActionParameterViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDActionParameterUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDActionParameterViewModel* model);
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
	TWeakObjectPtr<UTextBlock> ActionTitleTextWidget;
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
	TWeakObjectPtr<UUDActionParameterViewModel> ViewModel;
};