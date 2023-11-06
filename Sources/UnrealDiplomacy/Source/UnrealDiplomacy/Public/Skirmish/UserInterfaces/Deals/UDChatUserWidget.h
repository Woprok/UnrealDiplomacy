// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDChatUserWidget.generated.h"

// Forward Declarations

class UMultiLineEditableTextBox;
class UTextBlock;
class UButton;
class UUDListView;
class UUDChatViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDChatUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDChatViewModel* model);
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
	TWeakObjectPtr<UTextBlock> ChatTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> SendTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UMultiLineEditableTextBox> EditableMessageTextBoxWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> SendButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDListView> ChatItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDChatViewModel> ViewModel;
};