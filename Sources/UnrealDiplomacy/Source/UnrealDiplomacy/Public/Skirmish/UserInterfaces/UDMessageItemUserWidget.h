// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "UDMessageItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDMessageItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDMessageItemUserWidget : public UUDContentUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDMessageItemViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> AcceptButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> OkButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> RejectButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDMessageItemViewModel> ViewModel;
};