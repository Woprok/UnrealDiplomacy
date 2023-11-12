// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDCreateGameUserWidget.generated.h"

// Forward Declarations

class UCheckBox;
class UEditableTextBox;
class UButton;
class UUDCreateGameViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDCreateGameUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDCreateGameViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UCheckBox> IsLANCheckBoxWidget;
	UPROPERTY()
	TWeakObjectPtr<UEditableTextBox> SessionNameEditableTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> BackButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> NewGameButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDCreateGameViewModel> ViewModel;
};