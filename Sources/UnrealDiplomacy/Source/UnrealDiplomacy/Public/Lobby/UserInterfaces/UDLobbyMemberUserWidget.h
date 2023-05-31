// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDLobbyMemberUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UEditableTextBox;
class UTileView;
class UUDLobbyMemberViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDLobbyMemberUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDLobbyMemberViewModel* model);
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
	TWeakObjectPtr<UTextBlock> MemberSettingsTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NationNameTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> StrategyTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UEditableTextBox> NationNameEditableTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTileView> StrategyOptionTilesWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDLobbyMemberViewModel> ViewModel;
};