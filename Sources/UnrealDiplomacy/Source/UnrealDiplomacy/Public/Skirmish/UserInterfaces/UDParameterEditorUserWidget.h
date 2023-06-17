// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDParameterEditorUserWidget.generated.h"

// Forward Declarations

class UTileView;
class UUDParameterEditorViewModel;
class UUDTextParameterViewModel;
class UUDFactionParameterViewModel;
class UUDActionParameterViewModel;
class UUDTileParameterViewModel;
class UUDValueParameterViewModel;
class UUDResourceParameterViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDParameterEditorUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDParameterEditorViewModel* model);
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
	/** Callback to set data from view model. */
	void SetFactionParameterCollection(const TArray<TObjectPtr<UUDFactionParameterViewModel>>& parameterViewModels);
	/** Callback to set data from view model. */
	void SetActionParameterCollection(const TArray<TObjectPtr<UUDActionParameterViewModel>>& parameterViewModels);
	/** Callback to set data from view model. */
	void SetTileParameterCollection(const TArray<TObjectPtr<UUDTileParameterViewModel>>& parameterViewModels);
	/** Callback to set data from view model. */
	void SetValueParameterCollection(const TArray<TObjectPtr<UUDValueParameterViewModel>>& parameterViewModels);
	/** Callback to set data from view model. */
	void SetTextParameterCollection(const TArray<TObjectPtr<UUDTextParameterViewModel>>& parameterViewModels);
	/** Callback to set data from view model. */
	void SetResourceParameterCollection(const TArray<TObjectPtr<UUDResourceParameterViewModel>>& parameterViewModels);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTileView> FactionParameterListWidget;
	UPROPERTY()
	TWeakObjectPtr<UTileView> TileParameterListWidget;
	UPROPERTY()
	TWeakObjectPtr<UTileView> TextParameterListWidget;
	UPROPERTY()
	TWeakObjectPtr<UTileView> ResourceParameterListWidget;
	UPROPERTY()
	TWeakObjectPtr<UTileView> ValueParameterListWidget;
	UPROPERTY()
	TWeakObjectPtr<UTileView> ActionParameterListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDParameterEditorViewModel> ViewModel;
};