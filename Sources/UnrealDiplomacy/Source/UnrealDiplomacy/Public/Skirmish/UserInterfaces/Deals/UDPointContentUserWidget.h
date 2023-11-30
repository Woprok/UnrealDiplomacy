// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "UDPointContentUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDPointContentViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDPointContentUserWidget : public UUDContentUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDPointContentViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
protected:
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDPointContentViewModel> ViewModel;
};