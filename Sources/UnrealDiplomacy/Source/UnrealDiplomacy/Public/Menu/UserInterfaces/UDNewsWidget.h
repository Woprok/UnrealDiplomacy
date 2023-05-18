// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDNewsWidget.generated.h"

// Forward Declaratiosn

class UTextBlock;
class UButton;
class UUDNewsViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDNewsWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables to define view model used by this widet for delegates.
	 */
	virtual void SetViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDNewsViewModel* model);
protected:
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
	TWeakObjectPtr<UTextBlock> NewsTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> MessageTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CloseTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CloseButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDNewsViewModel> ViewModel;
private:
	// Button Functions
	UFUNCTION()
	void Close();
};