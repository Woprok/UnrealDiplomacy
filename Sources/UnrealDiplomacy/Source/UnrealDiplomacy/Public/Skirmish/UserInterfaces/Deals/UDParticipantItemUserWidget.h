// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDListEntryUserWidget.h"
#include "UDParticipantItemUserWidget.generated.h"

// Forward Declarations

class UButton;
class UUDParticipantItemViewModel;

/**
 * Ancestor for blueprint.
 * Supports IUserObjectListEntry for list view initializations.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDParticipantItemUserWidget : public UUDListEntryUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDParticipantItemViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
protected:
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDParticipantItemViewModel> ViewModel;
};