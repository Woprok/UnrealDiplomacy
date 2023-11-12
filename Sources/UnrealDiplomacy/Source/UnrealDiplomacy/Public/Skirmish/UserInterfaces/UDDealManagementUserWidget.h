// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDDealManagementUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDDealManagementViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealManagementUserWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDealManagementViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
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
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealManagementViewModel> ViewModel;
};