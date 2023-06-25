// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDUserWidget.h"
#include "UDDealGeneralTabUserWidget.generated.h"

// Forward Declarations

class UTextBlock;
class UButton;
class UListView;
class UUDChatUserWidget;
class UUDChatViewModel;
class UUDDealGeneralTabViewModel;
class UUDParticipantItemViewModel;
class UUDInviteItemViewModel;

/**
 * Ancestor for blueprint.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealGeneralTabUserWidget : public UUDUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * Enables blueprint to bind view model.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void SetBlueprintViewModel(UUDDealGeneralTabViewModel* model);
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
	void SetParticipantSourceCollection(const TArray<TObjectPtr<UUDParticipantItemViewModel>>& itemViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetInviteSourceCollection(const TArray<TObjectPtr<UUDInviteItemViewModel>>& itemViewModels);
	/**
	 * Callback to set data from view model.
	 */
	void SetChatSourceInstance(const TObjectPtr<UUDChatViewModel>& chatViewModel);
protected:
	// Bindings
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> StateTitleTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> StateTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ParticipantTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ReadyCountTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> VoteCountTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> InviteableTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UTextBlock> ReadyTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> VoteTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> LeaveTextWidget;
	UPROPERTY()
	TWeakObjectPtr<UTextBlock> CancelTextWidget;

	UPROPERTY()
	TWeakObjectPtr<UButton> ChangeReadyButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> ChangeVoteButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> LeaveButtonWidget;
	UPROPERTY()
	TWeakObjectPtr<UButton> CancelButtonWidget;

	UPROPERTY()
	TWeakObjectPtr<UListView> ParticipantItemListWidget;
	UPROPERTY()
	TWeakObjectPtr<UListView> InviteItemListWidget;
	UPROPERTY()
	TWeakObjectPtr<UUDChatUserWidget> DealChatViewWidget;
	// ViewModel
	UPROPERTY()
	TWeakObjectPtr<UUDDealGeneralTabViewModel> ViewModel;
};