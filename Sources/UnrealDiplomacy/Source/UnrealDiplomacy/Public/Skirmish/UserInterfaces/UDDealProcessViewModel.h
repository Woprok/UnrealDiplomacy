// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "UDDealProcessViewModel.generated.h"

#define LOCTEXT_NAMESPACE "ActionUI"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDParticipantsUpdated, FUDDealParticipantsInfo, infos);

UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDParticipantViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Name;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsParticipant;
private:
	FUDPlayerInfo CurrentInfo;
	int32 CurrentDealUniqueId;
public:
	UFUNCTION(BlueprintCallable)
	void SetBindingTarget(FUDPlayerInfo info, int32 dealUniqueId)
	{
		CurrentInfo = info;
		CurrentDealUniqueId = dealUniqueId;
		auto rs1 = FText::Format(
			LOCTEXT("Participant", "Player {0}"),
			info.Id
		).ToString();
		SetName(rs1);
		SetIsParticipant(ActionModel->IsParticipantInCurrentDeal(dealUniqueId, info.Id));
	}

	UFUNCTION(BlueprintCallable)
	void InvitePlayer()
	{
		ActionModel->RequestAction(ActionModel->GetInviteParticipantDealAction(CurrentDealUniqueId, CurrentInfo.Id));
	}
	UFUNCTION(BlueprintCallable)
	void LeaveDeal()
	{
		ActionModel->RequestAction(ActionModel->GetLeaveParticipantDealAction(CurrentDealUniqueId, CurrentInfo.Id));
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetName(FString newName)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Name, newName);
	}
	/**
	 * MVVM Binding.
	 */
	void SetIsParticipant(bool newIsParticipant)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsParticipant, newIsParticipant);
	}
	/**
	 * MVVM Binding.
	 */
	FString GetName() const
	{
		return Name;
	}
	/**
	 * MVVM Binding.
	 */
	bool GetIsParticipant() const
	{
		return IsParticipant;
	}
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDDealProcessViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
  	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString SessionDescription;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsSessionActive;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentDeal = 0;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsEnabledPreviousDeal = 0;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsEnabledNextDeal = 0;
protected:
	/**
	 * Deals sorted by time they were created.
	 */
	TArray<int32> DealsByTimeline;
	/**
	 * Current deal displayed by viewmodel.
	 */
	FUDDealInfo CurrentDealItem;
	/**
	 * Current deal displayed by viewmodel.
	 */
	int32 CurrentIndex;

public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			TArray<int32> deals = ActionModel->GetDealIds();
			if (deals.Num() > 0)
			{
				UpdateInteractions(deals);
			}
			else
			{
				// this will happen only once ?
				OnDealEmpty(ActionModel->GetDealInfoAnyDEBUG());
			}
		}
		else
		{
			// This should not be visible ?
		}
	}
	/**
	 * Invoked when this requires view to rebind.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDParticipantsUpdated ParticipantsOnUpdated;

	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemPrevious()
	{
		int32 prev = FMath::Max(0, CurrentIndex - 1);
		if (prev == CurrentIndex)
		{
			prev = DealsByTimeline.Num() - 1;
		}
		UpdateView(DealsByTimeline.Num(), DealsByTimeline[prev], prev);
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemReject()
	{
		//if (CurrentItem.ActionTypeId == UUDGiftAction::ActionTypeId)
		//	ActionModel->RequestAction(ActionModel->GetRejectConditionalGiftGoldAction(CurrentItem));
		//else
		//	ActionModel->RequestAction(ActionModel->GetRejectParticipantDealAction(CurrentItem));
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemAccept()
	{
		//if (CurrentItem.ActionTypeId == UUDGiftAction::ActionTypeId)
		//	ActionModel->RequestAction(ActionModel->GetConfirmConditionalGiftGoldAction(CurrentItem));
		//else
		//	ActionModel->RequestAction(ActionModel->GetAcceptParticipantDealAction(CurrentItem));

	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void ItemNext()
	{
		int32 next = FMath::Min(DealsByTimeline.Num() - 1, CurrentIndex + 1);
		if (next == CurrentIndex)
		{
			next = 0;
		}
		UpdateView(DealsByTimeline.Num(), DealsByTimeline[next], next);
	}
	UFUNCTION(BlueprintCallable)
	void ItemClose()
	{
		ActionModel->RequestAction(ActionModel->GetCloseDealAction(CurrentDealItem.DealUniqueId));
	}

protected:
	void UpdateView(int32 dealHistoryCount, int32 dealUniqueId, int32 arrayPosition)
	{
		CurrentDealItem = ActionModel->GetDealInfo(dealUniqueId);
		CurrentDeal = dealUniqueId;
		CurrentIndex = arrayPosition;

		if (dealHistoryCount > 0)
		{
			SetIsEnabledPreviousDeal(true);
			SetIsEnabledNextDeal(true);
		}
		else
		{
			SetIsEnabledPreviousDeal(false);
			SetIsEnabledNextDeal(false);
		}

		OnDealPresent(CurrentDealItem);
		/*
		SetPendingCount(dealHistoryCount);
		SetCurrentItem(data); //at least one must be present...
		SetCurrentItemShown(arrayPosition);
		auto rs1 = FText(
			LOCTEXT("Gift", "Gift")
		).ToString();
		SetCurrentItemName(rs1);
		auto rs2 = FText::Format(
			LOCTEXT("Gift", "A{0} Player {1} offered {2} as gift. Do we accept ?"),
			data.ActionTypeId,
			data.InvokerPlayerId,
			data.ValueParameter
		).ToString();
		SetCurrentItemDescription(rs2);*/
	}
	
	void UpdateInteractions(TArray<int32> deals)
	{
		if (CurrentDeal == 0 && deals.Num() > 0)
		{
			UpdateView(deals.Num(), deals[0], 0);
			// otherwise there is nothing to show yet...
		}
		// list already contains item from previous update
		// current list will be updated to new list, preserving selection
		// note: CurrentDeal != 0 is used as deal is eligible to have negative id.
		else if (CurrentDeal != 0 && deals.Num() > 0)
		{
			// find current deal in new array
			int32 oldItemPosition = deals.Find(CurrentDeal);

			// set old back
			if (oldItemPosition != -1)
			{
				UpdateView(deals.Num(), deals[oldItemPosition], oldItemPosition);
			}
			// fallback to start (delete is unlikely to happen)
			else
			{
				UpdateView(deals.Num(), deals[0], 0);
			}
		}
		// list is now empty, so we define empty item as current... (TODO skip empty item phase ?)
		else // in case of CurrentDeal == 0 and deals.Num() == 0
		{
			// First initialization, empty deal is only deal that can have 0
			// TODO this is only temporary fallback, in case we would accidently call it.
			OnDealEmpty(ActionModel->GetDealInfoAnyDEBUG());
		}
		// finally update the array
		DealsByTimeline = deals;
	}



	void OnDealEmpty(FUDDealInfo info)
	{
		SetIsEnabledPreviousDeal(false);
		SetIsEnabledNextDeal(false);
		SetIsSessionActive(false);
		auto rs1 = FText(
			LOCTEXT("DealState", "No deal is currently discussed with you.")
		).ToString();
		SetSessionDescription(rs1);
	}
	void OnDealPresent(FUDDealInfo info)
	{
		SetIsSessionActive(true);
		auto rs1 = FText::Format(
			LOCTEXT("DealState", "Deal is currently discussed with you, ID{0}."),
			info.DealUniqueId
		).ToString();
		SetSessionDescription(rs1);

		auto data = ActionModel->GetDealParticipants(info.DealUniqueId);
		ParticipantsOnUpdated.Broadcast(data);

	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetSessionDescription(FString newSessionDescription)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(SessionDescription, newSessionDescription);
	}
	/**
	 * MVVM Binding.
	 */
	void SetIsSessionActive(bool newIsSessionActive)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsSessionActive, newIsSessionActive);
	}
	/**
	 * MVVM Binding.
	 */
	void SetCurrentDeal(int32 newCurrentDeal)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CurrentDeal, newCurrentDeal);
	}
	/**
	 * MVVM Binding.
	 */
	void SetIsEnabledPreviousDeal(bool newIsEnabledPreviousDeal)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsEnabledPreviousDeal, newIsEnabledPreviousDeal);
	}
	/**
	 * MVVM Binding.
	 */
	void SetIsEnabledNextDeal(bool newIsEnabledNextDeal)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsEnabledNextDeal, newIsEnabledNextDeal);
	}
	/**
	 * MVVM Binding.
	 */
	FString GetSessionDescription() const
	{
		return SessionDescription;
	}	
	/**
	 * MVVM Binding.
	 */
	bool GetIsSessionActive() const
	{
		return IsSessionActive;
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetCurrentDeal() const
	{
		return CurrentDeal;
	}
	/**
	 * MVVM Binding.
	 */
	bool GetIsEnabledPreviousDeal() const
	{
		return IsEnabledPreviousDeal;
	}
	/**
	 * MVVM Binding.
	 */
	bool GetIsEnabledNextDeal() const
	{
		return IsEnabledNextDeal;
	}
};
#undef LOCTEXT_NAMESPACE