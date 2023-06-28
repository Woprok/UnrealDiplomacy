// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDActionItemUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDActionItemViewModel;
class UUDParameterEditorUserWidget;
class UUDParameterEditorViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDActionItemUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDActionItemViewModel* model);
protected:
	/**
	 * Allows List View to define context.
	 */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
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
	void SetParameterEditorSourceInstance(const TObjectPtr<UUDParameterEditorViewModel>& parameterEditorViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ActionTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ActionTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> AcceptTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ChangeTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> DenyTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> SabotageTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> EditorTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> AcceptButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ChangeButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> DenyButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> SabotageButtonWidget;

	UPROPERTY()
	TWeakObjectPtr<UUDParameterEditorUserWidget> ParameterEditorViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDActionItemViewModel> ViewModel;
};