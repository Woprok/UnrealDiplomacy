// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDMessageManagementViewModel.generated.h"

// Forward Declarations

struct FUDMessageInfo;
struct FUDMessageInteractionInfo;
class UUDMessageItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDMessageItemChanged, const TObjectPtr<UUDMessageItemViewModel>& messageItemViewModel);

/**
 * Browsing of all interactable action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDMessageManagementViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void Close();
	UFUNCTION()
	void First();
	UFUNCTION()
	void Previous();
	UFUNCTION()
	void Next();
	UFUNCTION()
	void Last();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageManagementTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText MessageCountText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CloseText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText FirstText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PreviousText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NextText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LastText;
	// Events
	FUDMessageItemChanged MessageItemChangedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates message item to latest.
	 */
	void UpdateMessageItems();
	/**
	 * Updates message item to latest.
	 */
	void UpdateSelectedMessageItem();
	/**
	 * Tries to retrieve desired selected item, on fail returns first element.
	 * If messages list is empty returns invalid item.
	 */
	FUDMessageInfo GetSelectedOrDefault(int32 desiredSelectedItem);
private:
	// MVVM Setters & Getters
	void SetMessageManagementTitleText(FText newMessageManagementTitleText);
	FText GetMessageManagementTitleText() const;
	void SetMessageCountText(FText newMessageCountText);
	FText GetMessageCountText() const;
	void SetCloseText(FText newCloseText);
	FText GetCloseText() const;
	void SetFirstText(FText newFirstText);
	FText GetFirstText() const;
	void SetPreviousText(FText newPreviousText);
	FText GetPreviousText() const;
	void SetNextText(FText newNextText);
	FText GetNextText() const;
	void SetLastText(FText newLastText);
	FText GetLastText() const;
private:
	// Fields
	int32 SelectedIndex;
	FUDMessageInfo SelectedMessageItem;
	FUDMessageInteractionInfo Content;
	// Current Instance in use...
	FName MessageItemInstanceName = TEXT("MessageItemInstance");
	TSubclassOf<UUDViewModel> MessageItemType;
	TObjectPtr<UUDMessageItemViewModel> MessageItemInstance;
};

/*/
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteAccept.h"
#include "Core/Simulation/Actions/UDGameActionTileTransferAccept.h"
#include "Core/Simulation/Actions/UDGameActionGiftReject.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteReject.h"
#include "Core/Simulation/Actions/UDGameActionTileTransferReject.h"
	int32 PendingCount = -1;
	FString CurrentItemName;
	FString CurrentItemDescription;
	FUDActionData CurrentItem;
	int32 CurrentItemShown;
	TArray<FUDActionData> LastRequestArray;
	virtual void OnUpdate() override
	{
			// This is visible during the game and shows current info
			UpdateInteractions(ActionModel->GetPendingRequests());
	}

protected:
	//requests.Num()
	//requests[0]
	//position in current (new) list
	void UpdateView(int32 count, FUDActionData action, int32 position)
	{
		SetPendingCount(count);
		SetCurrentItem(action); //at least one must be present...
		SetCurrentItemShown(position);
		auto rs1 = FText(
			LOCTEXT("Gift", "Gift")
		).ToString();
		SetCurrentItemName(rs1);
		auto rs2 = FText::Format(
			LOCTEXT("Gift", "A{0} Player {1} offered gift. Do we accept ?"),
			action.ActionTypeId,
			action.InvokerFactionId
		).ToString();
		SetCurrentItemDescription(rs2);
	}
	void UpdateInteractions(TArray<FUDActionData> requests)
	{
		// First initialization or no item
		if (PendingCount == -1 && requests.Num() > 0)
		{
			UpdateView(requests.Num(), requests[0], 0);
			// otherwise there is nothing to show yet...
		}
		// last present was completed and we did not receive any new...
		else if (PendingCount == 0 && requests.Num() > 0)
		{
			UpdateView(requests.Num(), requests[0], 0);
		}
		// list is still containing items in previous iteration
		// current list contains at least one item...
		else if (PendingCount > 0 && requests.Num() > 0)
		{
			int32 oldItemPosition = -1;
			for (int32 pos = 0; pos < requests.Num(); pos++)
			{
				if (requests[pos].UniqueId == CurrentItem.UniqueId)
				{
					oldItemPosition = pos;
					break;
				}
			}

			// set old back
			if (oldItemPosition != - 1)
			{
				UpdateView(requests.Num(), requests[oldItemPosition], oldItemPosition);
			}
			// show next or last, if old was last
			else
			{
				int32 next = FMath::Min(requests.Num() - 1, CurrentItemShown);
				UpdateView(requests.Num(), requests[next], next);
			}
		}
		// list is now empty, so we define empty item as current... (TODO skip empty item phase ?)
		else
		{
			UpdateView(0, FUDActionData(), 0);
		}
		// finally update the array
		LastRequestArray = requests;
	}
private:
UFUNCTION(BlueprintCallable)
void ItemReject()
{
	if (CurrentItem.ActionTypeId == UUDGameActionGift::ActionTypeId)
		ActionModel->RequestAction(ActionModel->GetRejectAction(UUDGameActionGiftReject::ActionTypeId, CurrentItem));
	else if (CurrentItem.ActionTypeId == UUDDealActionParticipantInvite::ActionTypeId)
		ActionModel->RequestAction(ActionModel->GetRejectAction(UUDDealActionParticipantInviteReject::ActionTypeId, CurrentItem));
	else if (CurrentItem.ActionTypeId == UUDGameActionTileTransfer::ActionTypeId)
		ActionModel->RequestAction(ActionModel->GetRejectAction(UUDGameActionTileTransferReject::ActionTypeId, CurrentItem));
}
UFUNCTION(BlueprintCallable)
void ItemAccept()
{
	if (CurrentItem.ActionTypeId == UUDGameActionGift::ActionTypeId)
		ActionModel->RequestAction(ActionModel->GetRejectAction(UUDGameActionGiftAccept::ActionTypeId, CurrentItem));
	else if (CurrentItem.ActionTypeId == UUDDealActionParticipantInvite::ActionTypeId)
		ActionModel->RequestAction(ActionModel->GetRejectAction(UUDDealActionParticipantInviteAccept::ActionTypeId, CurrentItem));
	else if (CurrentItem.ActionTypeId == UUDGameActionTileTransfer::ActionTypeId)
		ActionModel->RequestAction(ActionModel->GetRejectAction(UUDGameActionTileTransferAccept::ActionTypeId, CurrentItem));
}

/*/