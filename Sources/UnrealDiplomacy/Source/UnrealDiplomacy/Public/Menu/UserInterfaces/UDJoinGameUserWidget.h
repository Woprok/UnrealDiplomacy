// Copyright Miroslav Valach
// TODO Create list view that has exposed events (hover, select, click) to c++

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDJoinGameUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDJoinGameViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDJoinGameUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDJoinGameViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> BackButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> RefreshButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> DirectConnectButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDJoinGameViewModel> ViewModel;
};