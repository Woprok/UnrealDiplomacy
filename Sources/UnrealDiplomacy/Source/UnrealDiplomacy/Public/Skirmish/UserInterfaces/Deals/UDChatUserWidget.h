// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "UDChatUserWidget.generated.h"

// Forward Declarations

class UMultiLineEditableTextBox;
class UButton;
class UUDChatViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDChatUserWidget : public UUDContentUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDChatViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UMultiLineEditableTextBox> EditableMessageTextBoxWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> SendButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDChatViewModel> ViewModel;
};