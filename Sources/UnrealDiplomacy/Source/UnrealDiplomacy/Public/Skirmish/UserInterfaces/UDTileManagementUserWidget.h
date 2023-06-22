// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDTileManagementUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UListView;
class UUDTileManagementViewModel;
class UUDTileInteractionViewModel;
class UUDModifierItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDTileManagementUserWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDTileManagementViewModel* model);
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
	void SetTileInteractionSourceCollection(const TArray<TObjectPtr<UUDTileInteractionViewModel>>& tileInteractionViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetModifierItemSourceCollection(const TArray<TObjectPtr<UUDModifierItemViewModel>>& modifierItemViewModels);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> TileManagementTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> TileNameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> OwnerTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> FactionNameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ResourceTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ResourceValueTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ResourceTypeTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CloseTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CloseButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> ModifierItemListWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> TileInteractionListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDTileManagementViewModel> ViewModel;
};