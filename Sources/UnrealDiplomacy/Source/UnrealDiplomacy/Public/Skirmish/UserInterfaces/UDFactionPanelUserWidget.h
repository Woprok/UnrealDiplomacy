// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDFactionPanelUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UListView;
class UUDFactionPanelViewModel;
class UUDFactionItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDFactionPanelUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDFactionPanelViewModel* model);
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
	void SetFactionSourceCollection(const TArray<TObjectPtr<UUDFactionItemViewModel>>& factionItemViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NameHeaderTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ControllerHeaderTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> FactionItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDFactionPanelViewModel> ViewModel;
};