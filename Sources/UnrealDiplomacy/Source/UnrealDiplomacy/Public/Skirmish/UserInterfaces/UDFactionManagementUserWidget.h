// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDWindowUserWidget.h"
#include "UDFactionManagementUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDFactionManagementViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDFactionManagementUserWidget : public UUDWindowUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDFactionManagementViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UButton> CloseButtonWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDFactionManagementViewModel> ViewModel;
};