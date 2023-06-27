// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDParameterEditorUserWidget.generated.h"

// Forward Declarations

class UUDParameterEditorViewModel;
class UUDFactionParameterUserWidget;
class UUDTileParameterUserWidget;
class UUDActionParameterUserWidget;
class UUDValueParameterUserWidget;
class UUDResourceParameterUserWidget;
class UUDTextParameterUserWidget;
class UUDParameterEditorViewModel;
class UUDFactionParameterViewModel;
class UUDTileParameterViewModel;
class UUDActionParameterViewModel;
class UUDValueParameterViewModel;
class UUDResourceParameterViewModel;
class UUDTextParameterViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDParameterEditorUserWidget : public UUDUserWidget
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
	void SetDealActionParameterInstance(const TObjectPtr<UUDActionParameterViewModel>& parameterViewModel);
	/** Callback to set data from view model. */
	void SetFactionTargetParameterInstance(const TObjectPtr<UUDFactionParameterViewModel>& parameterViewModel);
	/** Callback to set data from view model. */
	void SetFactionInvokerParameterInstance(const TObjectPtr<UUDFactionParameterViewModel>& parameterViewModel);
	/** Callback to set data from view model. */
	void SetTileParameterInstance(const TObjectPtr<UUDTileParameterViewModel>& parameterViewModel);
	/** Callback to set data from view model. */
	void SetActionParameterInstance(const TObjectPtr<UUDActionParameterViewModel>& parameterViewModel);
	/** Callback to set data from view model. */
	void SetResourceParameterInstance(const TObjectPtr<UUDResourceParameterViewModel>& parameterViewModel);
	/** Callback to set data from view model. */
	void SetValueParameterInstance(const TObjectPtr<UUDValueParameterViewModel>& parameterViewModel);
	/** Callback to set data from view model. */
	void SetTextParameterInstance(const TObjectPtr<UUDTextParameterViewModel>& parameterViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UUDActionParameterUserWidget> DealActionParameterWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDFactionParameterUserWidget> FactionInvokerParameterWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDFactionParameterUserWidget> FactionTargetParameterWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDTileParameterUserWidget> TileParameterWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDActionParameterUserWidget> ActionParameterWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDResourceParameterUserWidget> ResourceParameterWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDValueParameterUserWidget> ValueParameterWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDTextParameterUserWidget> TextParameterWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDParameterEditorViewModel> ViewModel;
};