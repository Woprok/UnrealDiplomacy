// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDLobbyUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UListView;
class UUDLobbyHostUserWidget;
class UUDLobbyMemberUserWidget;
class UUDLobbyHostViewModel;
class UUDLobbyMemberViewModel;
class UUDClientItemViewModel;
class UUDLobbyViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDLobbyUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDLobbyViewModel* model);
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
	void SetClientSourceCollection(const TArray<TObjectPtr<UUDClientItemViewModel>>& clientItemViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetHostSourceInstance(const TObjectPtr<UUDLobbyHostViewModel>& lobbyHostViewModel);
	/**
	 * Callback to set data from view model.
	 */
	void SetMemberSourceInstance(const TObjectPtr<UUDLobbyMemberViewModel>& lobbyMemberViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> LobbyTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> NameHeaderTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> BackTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> LeaveTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> StartTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> BackButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> LeaveButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> StartButtonWidget;

	UPROPERTY()
	TWeakObjectPtr<UUDLobbyHostUserWidget> LobbyHostViewWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDLobbyMemberUserWidget> LobbyMemberViewWidget;

	UPROPERTY()
	TWeakObjectPtr<UListView> ClientItemListWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDLobbyViewModel> ViewModel;
};