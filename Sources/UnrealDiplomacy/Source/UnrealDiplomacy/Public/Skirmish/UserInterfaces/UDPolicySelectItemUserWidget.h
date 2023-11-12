// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UDPolicySelectItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDPolicySelectItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDPolicySelectItemUserWidget : public UUDUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDPolicySelectItemViewModel* model);
protected:
	/** Allows List View to define context. */
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> SelectButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDPolicySelectItemViewModel> ViewModel;
};