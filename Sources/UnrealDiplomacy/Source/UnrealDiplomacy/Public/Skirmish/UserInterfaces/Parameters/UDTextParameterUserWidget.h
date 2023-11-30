// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "UDTextParameterUserWidget.generated.h"

// Forward Declarations

class UEditableTextBox;
class UUDTextParameterViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDTextParameterUserWidget : public UUDContentUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDTextParameterViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UEditableTextBox> TextEditableTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UEditableTextBox> EditableTextBoxWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDTextParameterViewModel> ViewModel;
};