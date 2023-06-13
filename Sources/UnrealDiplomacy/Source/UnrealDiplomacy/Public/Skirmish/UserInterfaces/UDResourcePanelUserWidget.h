// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDResourcePanelUserWidget.generated.h"

// Forward Declarations

class UListView;
class UUDResourcePanelViewModel;
class UUDResourceItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDResourcePanelUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDResourcePanelViewModel* model);
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
	void SetFactionSourceCollection(const TArray<TObjectPtr<UUDResourceItemViewModel>>& resourceItemViewModels);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UListView> ResourceItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDResourcePanelViewModel> ViewModel;
};