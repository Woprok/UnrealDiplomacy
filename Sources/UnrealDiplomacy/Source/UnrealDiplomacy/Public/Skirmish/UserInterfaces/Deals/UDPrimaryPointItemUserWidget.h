// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDPrimaryPointItemUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UListView;
class UUDPrimaryPointItemViewModel;
class UUDSecondaryPointItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDPrimaryPointItemUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDPrimaryPointItemViewModel* model);
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
	void SetPointItemSourceCollection(const TArray<TObjectPtr<UUDSecondaryPointItemViewModel>>& itemViewModels);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CreatePointTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CreatePointButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> PointItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDPrimaryPointItemViewModel> ViewModel;
};