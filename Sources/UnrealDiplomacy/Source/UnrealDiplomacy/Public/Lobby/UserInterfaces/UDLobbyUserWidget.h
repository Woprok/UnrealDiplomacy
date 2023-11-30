// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDLobbyUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDLobbyViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDLobbyUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDLobbyViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> BackButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> LeaveButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> StartButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDLobbyViewModel> ViewModel;
};