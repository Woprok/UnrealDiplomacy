// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDDialogueUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDDialogueViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDialogueUserWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDialogueViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> DialogueTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ContentTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CloseTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CloseButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDialogueViewModel> ViewModel;
};