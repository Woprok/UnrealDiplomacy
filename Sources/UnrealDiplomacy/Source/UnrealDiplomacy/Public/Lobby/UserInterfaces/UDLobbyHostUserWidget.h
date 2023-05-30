// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDLobbyHostUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UUDLobbyHostViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDLobbyHostUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDLobbyHostViewModel* model);
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
	TWeakObjectPtr<UTextBlock> HostSettingsTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> AICountTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> MapSeedTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> MapWidthTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> MapHeightTextWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDLobbyHostViewModel> ViewModel;
};