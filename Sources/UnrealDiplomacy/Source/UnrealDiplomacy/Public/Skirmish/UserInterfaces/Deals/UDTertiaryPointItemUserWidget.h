// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDTertiaryPointItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDTertiaryPointItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDTertiaryPointItemUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDTertiaryPointItemViewModel* model);
protected:
	/** Allows List View to define context. */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> CreateTertiaryPointButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDTertiaryPointItemViewModel> ViewModel;
};