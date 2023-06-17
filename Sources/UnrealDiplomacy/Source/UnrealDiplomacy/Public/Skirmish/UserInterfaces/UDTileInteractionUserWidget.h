// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDTileInteractionUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDParameterEditorUserWidget;
class UUDParameterEditorViewModel;
class UUDTileInteractionViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDTileInteractionUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDTileInteractionViewModel* model);
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
	TWeakObjectPtr<UTextBlock> InteractTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> InteractButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDParameterEditorUserWidget> ParameterEditorViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDTileInteractionViewModel> ViewModel;
};