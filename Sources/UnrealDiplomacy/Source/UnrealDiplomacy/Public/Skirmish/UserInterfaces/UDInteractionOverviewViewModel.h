// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/UDActionData.h"
#include "UDInteractionOverviewViewModel.generated.h"


#define LOCTEXT_NAMESPACE "ActionUI"
/**
 * Browsing of all currently available interactions...
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDInteractionOverviewViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 PendingCount = -1;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CurrentItemName;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CurrentItemDescription;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDActionData CurrentItem;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentItemShown;
private:
	TArray<FUDActionData> LastRequestArray;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// This is visible during the game and shows current info
			UpdateInteractions(ActionModel->GetPendingRequests());
		}
		else
		{
			// This should not be visible ?
		}
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemPrevious()
	{
		int32 prev = FMath::Max(0, CurrentItemShown - 1);
		if (prev == CurrentItemShown)
		{
			prev = LastRequestArray.Num() - 1;
		}
		UpdateView(LastRequestArray.Num(), LastRequestArray[prev], prev);
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemReject()
	{
		if (CurrentItem.ActionTypeId == UUDGameActionGift::ActionTypeId)		
			ActionModel->RequestAction(ActionModel->GetRejectConditionalGiftGoldAction(CurrentItem));
		else if (CurrentItem.ActionTypeId == UUDDealActionParticipantInvite::ActionTypeId)
			ActionModel->RequestAction(ActionModel->GetRejectParticipantDealAction(CurrentItem));
		else if (CurrentItem.ActionTypeId == UUDGameActionTileTransfer::ActionTypeId)
			ActionModel->RequestAction(ActionModel->GetRejectTransferTileAction(CurrentItem));
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemAccept()
	{
		if (CurrentItem.ActionTypeId == UUDGameActionGift::ActionTypeId)
			ActionModel->RequestAction(ActionModel->GetConfirmConditionalGiftGoldAction(CurrentItem));
		else if (CurrentItem.ActionTypeId == UUDDealActionParticipantInvite::ActionTypeId)
			ActionModel->RequestAction(ActionModel->GetAcceptParticipantDealAction(CurrentItem));
		else if (CurrentItem.ActionTypeId == UUDGameActionTileTransfer::ActionTypeId)
			ActionModel->RequestAction(ActionModel->GetConfirmTransferTileAction(CurrentItem));

	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemNext()
	{
		int32 next = FMath::Min(LastRequestArray.Num() - 1, CurrentItemShown + 1);
		if (next == CurrentItemShown)
		{
			next = 0;
		}
		UpdateView(LastRequestArray.Num(), LastRequestArray[next], next);
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
			action.InvokerPlayerId
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
	/**
	 * MVVM Binding.
	 */
	void SetPendingCount(int32 newPendingCount)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(PendingCount, newPendingCount);
	}
	/**
	 * MVVM Binding.
	 */
	void SetCurrentItemName(FString newCurrentItemName)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CurrentItemName, newCurrentItemName);
	}
	/**
	 * MVVM Binding.
	 */
	void SetCurrentItemDescription(FString newCurrentItemDescription)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CurrentItemDescription, newCurrentItemDescription);
	}
	/**
	 * MVVM Binding.
	 */
	void SetCurrentItem(FUDActionData newCurrentItem)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CurrentItem, newCurrentItem);
	}
	/**
	 * MVVM Binding.
	 */
	void SetCurrentItemShown(int32 newCurrentItemShown)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CurrentItemShown, newCurrentItemShown);
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetPendingCount() const
	{
		return PendingCount;
	}
	/**
	 * MVVM Binding.
	 */
	FString GetCurrentItemName() const
	{
		return CurrentItemName;
	}
	/**
	 * MVVM Binding.
	 */
	FString GetCurrentItemDescription() const
	{
		return CurrentItemDescription;
	}
	/**
	 * MVVM Binding.
	 */
	FUDActionData GetCurrentItem() const
	{
		return CurrentItem;
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetCurrentItemShown() const
	{
		return CurrentItemShown;
	}
};
#undef LOCTEXT_NAMESPACE