// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDMenuUserWidget.generated.h"

// Fordward Declarations

class UButton;
class UUDMenuViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDMenuUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDMenuViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> CreateGameButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> JoinGameButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> SettingsButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> QuitButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDMenuViewModel> ViewModel;
};