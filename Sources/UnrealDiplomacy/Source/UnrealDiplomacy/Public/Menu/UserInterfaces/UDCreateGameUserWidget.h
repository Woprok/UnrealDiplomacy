// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDCreateGameUserWidget.generated.h"

// Forward Declarations

class UCheckBox;
class UEditableTextBox;
class UTextBlock;
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
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDCreateGameViewModel* model);
protected:
	/**
	 * Enables to define view model used by this widet for delegates.
	 */
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindDelegates() override;
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CreateGameTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> BackTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NewGameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> SessionNameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> IsLANTextWidget;
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