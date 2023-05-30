// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDStrategyOptionUserWidget.generated.h"

// Forward Declarations

class UImage;
class UUDStrategyOptionViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for tile view initializations.
 * If used in list view it should manually call
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDStrategyOptionUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDStrategyOptionViewModel* model);
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
	TWeakObjectPtr<UImage> OptionImageWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDStrategyOptionViewModel> ViewModel;
};