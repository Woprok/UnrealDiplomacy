// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "UDDealItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDDealItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealItemUserWidget : public UUDContentUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDealItemViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> GeneralButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> EditationButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ExecutionButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealItemViewModel> ViewModel;
};