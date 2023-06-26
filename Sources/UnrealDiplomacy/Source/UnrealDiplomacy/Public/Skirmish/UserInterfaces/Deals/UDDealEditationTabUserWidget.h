// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDDealEditationTabUserWidget.generated.h"

// Forward Declarations

class UListView;
class UUDDealEditationTabViewModel;
class UUDPointItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealEditationTabUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDealEditationTabViewModel* model);
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
	void SetPointItemSourceCollection(const TArray<TObjectPtr<UUDPointItemViewModel>>& itemViewModels);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UListView> PointItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealEditationTabViewModel> ViewModel;
};