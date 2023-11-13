// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDListEntryUserWidget.h"
#include "UDSecondaryPointItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDSecondaryPointItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDSecondaryPointItemUserWidget : public UUDListEntryUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDSecondaryPointItemViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> CreateSecondaryPointButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDSecondaryPointItemViewModel> ViewModel;
};