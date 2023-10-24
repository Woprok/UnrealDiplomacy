// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDMessageItemUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UUDMessageItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDMessageItemUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDMessageItemViewModel* model);
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
	TWeakObjectPtr<UTextBlock> MessageTypeTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> MessageTitleTextWidget;
	UPROPERTY()

	TWeakObjectPtr<UTextBlock> MessageConfirmTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> MessageRejectTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> OnConfirmTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> OnRejectTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UTextBlock> AcceptTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> OkTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> RejectTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> AcceptButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> OkButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> RejectButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDMessageItemViewModel> ViewModel;
};