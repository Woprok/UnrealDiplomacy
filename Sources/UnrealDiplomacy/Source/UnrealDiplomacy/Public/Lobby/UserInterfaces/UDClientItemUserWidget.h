// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDClientItemUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UUDClientItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDClientItemUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDClientItemViewModel* model);
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
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NameTextWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDClientItemViewModel> ViewModel;
};