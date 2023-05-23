// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDJoinGameUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UListView;
class UUDJoinGameViewModel;
class UUDServerItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDJoinGameUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDJoinGameViewModel* model);
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
	/**
	 * Callback to set data from view model.
	 */
	void SetSourceCollection(const TArray<TObjectPtr<UUDServerItemViewModel>>& serverItemViewModels);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> JoinGameTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> BackTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> RefreshTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> BackButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> RefreshButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> ServerItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDJoinGameViewModel> ViewModel;
};