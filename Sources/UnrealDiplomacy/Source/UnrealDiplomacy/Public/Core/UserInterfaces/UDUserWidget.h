// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UDUserWidget.generated.h"

// Forward Declarations

class UUDViewModel;

/**
 * Common ancestors for all widgets to create common contract between view and view model.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables to define view model used by this widet.
	 */
	virtual void SetViewModel(TObjectPtr<UUDViewModel> viewModel);
protected:
	/**
	 * Used to call all initializations for widget.
	 */
	virtual void NativeConstruct() override;
	/**
	 * Automatically invoked by native construct.
	 */
	virtual void BindDelegates();
	/**
	 * Autimatically invoked by native construct.
	 */
	virtual void SetAppearance();
};