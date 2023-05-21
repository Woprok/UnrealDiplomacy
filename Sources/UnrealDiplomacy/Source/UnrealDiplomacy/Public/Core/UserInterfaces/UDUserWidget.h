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
	virtual void NativeOnInitialized() override;
	/**
	 * Used to call editor initializations for widget.
	 * This is executed during the runtime as well.
	 */
	virtual void NativePreConstruct() override;
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindDelegates();
	/**
	 * Automatically invoked by native on initialized.
	 */
	virtual void BindWidgets();
	/**
	 * Automatically invoked by native pre construct.
	 */
	virtual void SetAppearance();
	/**
	 * Retrieve widget based on name and casts it to the specified type.
	 * This will throw runtime error, if it can't find desired widget.
	 */
	template<class T>
	TWeakObjectPtr<T> GetWidget(const FName& widgetName)
	{
		TObjectPtr<UWidget> widget_ptr = GetWidgetFromName(widgetName);
		ensureMsgf(widget_ptr, TEXT("UUDUserWidget: '%s' not found!"), *widgetName.ToString());
		TObjectPtr<T> casted_ptr = CastChecked<T>(widget_ptr);
		ensureMsgf(casted_ptr, TEXT("UUDUserWidget: '%s' incorrect type!"), *widgetName.ToString());
		return casted_ptr;
	}
};