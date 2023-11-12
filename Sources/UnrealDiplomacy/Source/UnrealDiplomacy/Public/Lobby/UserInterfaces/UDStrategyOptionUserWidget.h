// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDStrategyOptionUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDStrategyOptionViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for tile view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDStrategyOptionUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDStrategyOptionViewModel* model);
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> OptionButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDStrategyOptionViewModel> ViewModel;
};