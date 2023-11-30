// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDContentUserWidget.h"
#include "UDLobbyHostUserWidget.generated.h"

// Forward Declarations

class USpinBox;
class UUDLobbyHostViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDLobbyHostUserWidget : public UUDContentUserWidget
{
	GENERATED_BODY()
public:
	/** Enables blueprint to bind view model. */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDLobbyHostViewModel* model);
protected:
	virtual void BindViewModel(TObjectPtr<UUDViewModel> viewModel) override;
	virtual void BindDelegates() override;
	virtual void BindWidgets() override;
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<USpinBox> AICountValueWidget;
	UPROPERTY()
	TWeakObjectPtr<USpinBox> MapSeedValueWidget;
	UPROPERTY()
	TWeakObjectPtr<USpinBox> MapWidthValueWidget;
	UPROPERTY()
	TWeakObjectPtr<USpinBox> MapHeightValueWidget;
	UPROPERTY()
	TWeakObjectPtr<USpinBox> StratagemPointsValueWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDLobbyHostViewModel> ViewModel;
};