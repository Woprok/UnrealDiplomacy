// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDListEntryUserWidget.h"
#include "UDFactionInteractionUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDFactionInteractionViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDFactionInteractionUserWidget : public UUDListEntryUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDFactionInteractionViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> InteractButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDFactionInteractionViewModel> ViewModel;
};