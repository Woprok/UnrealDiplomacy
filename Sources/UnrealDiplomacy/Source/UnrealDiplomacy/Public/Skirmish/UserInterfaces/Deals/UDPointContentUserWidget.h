// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDPointContentUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDPointContentViewModel;
class UUDParameterEditorUserWidget;
class UUDParameterEditorViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDPointContentUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDPointContentViewModel* model);
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
	/**
	 * Callback to set data from view model.
	 */
	void SetParameterEditorSourceInstance(const TObjectPtr<UUDParameterEditorViewModel>& parameterEditorViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> PointTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> PointTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> EditorTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UUDParameterEditorUserWidget> ParameterEditorViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDPointContentViewModel> ViewModel;
};