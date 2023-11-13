// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDListEntryUserWidget.h"
#include "UDTertiaryPointItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDTertiaryPointItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDTertiaryPointItemUserWidget : public UUDListEntryUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDTertiaryPointItemViewModel* model);
protected:
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