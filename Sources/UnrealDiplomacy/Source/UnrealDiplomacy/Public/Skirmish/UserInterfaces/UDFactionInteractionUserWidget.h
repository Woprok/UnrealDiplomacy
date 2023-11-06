// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDFactionInteractionUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDParameterEditorUserWidget;
class UUDParameterEditorViewModel;
class UUDFactionInteractionViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDFactionInteractionUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDFactionInteractionViewModel* model);
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
	TWeakObjectPtr<UButton> InteractButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDParameterEditorUserWidget> ParameterEditorViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDFactionInteractionViewModel> ViewModel;
};