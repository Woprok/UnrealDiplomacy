// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDListEntryUserWidget.h"
#include "UDActionItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDActionItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDActionItemUserWidget : public UUDListEntryUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDActionItemViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> AcceptButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ChangeButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> DenyButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> SabotageButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDActionItemViewModel> ViewModel;
};