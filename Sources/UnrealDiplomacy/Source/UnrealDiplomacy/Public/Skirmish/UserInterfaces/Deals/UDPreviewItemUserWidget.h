// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDListEntryUserWidget.h"
#include "UDPreviewItemUserWidget.generated.h"

// Forward Declarations

class UUDPreviewItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDPreviewItemUserWidget : public UUDListEntryUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDPreviewItemViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
protected:
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDPreviewItemViewModel> ViewModel;
};