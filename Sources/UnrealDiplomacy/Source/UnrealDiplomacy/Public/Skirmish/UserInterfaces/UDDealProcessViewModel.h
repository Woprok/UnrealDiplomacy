// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "UDDealProcessViewModel.generated.h"

#define LOCTEXT_NAMESPACE "ActionUI"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDParticipantsUpdated, FUDDealParticipantsInfo, infos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPDealPointUpdated, FUDDealPointTreeInfo, dealRoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDChatUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDEditedDealUpdated);

UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDPointParticipantViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Name;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsInvoker;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsTarget;
	// Fields.
	FUDPlayerInfo CurrentPlayer;
	FUDDealPointInfo CurrentPoint;
public:
	UFUNCTION(BlueprintCallable)
	void SetBindingTarget(FUDDealPointInfo dealInfo, FUDPlayerInfo playerInfo)
	{
		CurrentPoint = dealInfo;
		CurrentPlayer = playerInfo;

		auto rs1 = FText::Format(
			LOCTEXT("Participant", "Player {0}"),
			CurrentPlayer.Id
		).ToString();
		SetName(rs1);
		bool isInv = ActionModel->IsDealPointInvoker(CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id);
		SetIsInvoker(isInv);
		bool isTar = ActionModel->IsDealPointTarget(CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id);
		SetIsTarget(isTar);

	}
	UFUNCTION(BlueprintCallable)
	void AddToInvokers()
	{
		ActionModel->RequestAction(
			ActionModel->UpdateAddInvokerDiscussionPointAction(
				CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id));
	}
	UFUNCTION(BlueprintCallable)
	void RemoveFromInvokers()
	{
		ActionModel->RequestAction(
			ActionModel->UpdateRemoveInvokerDiscussionPointAction(
				CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id));
	}
	UFUNCTION(BlueprintCallable)
	void AddToTargets()
	{
		ActionModel->RequestAction(
			ActionModel->UpdateAddTargetDiscussionPointAction(
				CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id));
	}
	UFUNCTION(BlueprintCallable)
	void RemoveFromTargets()
	{
		ActionModel->RequestAction(
			ActionModel->UpdateRemoveTargetDiscussionPointAction(
				CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id));
	}
private:
	// MVVM Setters & Getters
	void SetName(FString newName)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Name, newName);
	}
	FString GetName() const
	{
		return Name;
	}
	void SetIsInvoker(bool newIsInvoker)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsInvoker, newIsInvoker);
	}
	bool GetIsInvoker() const
	{
		return IsInvoker;
	}
	void SetIsTarget(bool newIsTarget)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsTarget, newIsTarget);
	}
	bool GetIsTarget() const
	{
		return IsTarget;
	}
};


UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDPointViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Title;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Description;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString ParticipantList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString InvokerList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString TargetList;
	// Public Field
	UPROPERTY(BlueprintReadOnly)
	FUDDealPointInfo CurrentPoint;
private:
public:
	UFUNCTION(BlueprintCallable)
	void SetBindingTarget(FUDDealPointChildInfo info)
	{
		CurrentPoint = ActionModel->GetDealPointInfo(info.DealUniqueId, info.PointUniqueId);
		auto rs1 = FText::Format(
			LOCTEXT("Participant", "Point of {0} identified as {1}"),
			info.DealUniqueId,
			info.PointUniqueId
		).ToString();
		SetTitle(rs1);
	}

	/**
	 * Returns array of child points, that can be used for setting additional viewmodels that 
	 * are starting from this viewmodel.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDDealPointChildInfo> GetChildPoints()
	{
		return ActionModel->GetDealPointChildTree(CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId);
		// returns list of all subpoints converted for tree construction
	}
	UFUNCTION(BlueprintCallable)
	void ItemAddChildPoint()
	{
		ActionModel->RequestAction(ActionModel->CreateChildDiscussionPointAction(CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId));
	}
private:
	// MVVM Setters & Getters
	void SetTitle(FString newTitle)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Title, newTitle);
	}
	FString GetTitle() const
	{
		return Title;
	}
	void SetDescription(FString newDescription)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Description, newDescription);
	}
	FString GetDescription() const
	{
		return Description;
	}
	void SetParticipantList(FString newParticipantList)
	{
		UE_MVVM_SET_PROPERTY_VALUE(ParticipantList, newParticipantList);
	}
	FString GetParticipantList() const
	{
		return ParticipantList;
	}
	void SetInvokerList(FString newInvokerList)
	{
		UE_MVVM_SET_PROPERTY_VALUE(InvokerList, newInvokerList);
	}
	FString GetInvokerList() const
	{
		return InvokerList;
	}
	void SetTargetList(FString newTargetList)
	{
		UE_MVVM_SET_PROPERTY_VALUE(TargetList, newTargetList);
	}
	FString GetTargetList() const
	{
		return TargetList;
	}
};

USTRUCT(BlueprintType)
struct FUDNamedOption
{
	GENERATED_BODY()
public:
	FUDNamedOption() {}
	FUDNamedOption(FText name, int32 optionCode) 
		: Name(name), OptionCode(optionCode) {}
	UPROPERTY(BlueprintReadOnly)
	FText Name;
	UPROPERTY(BlueprintReadOnly)
	int32 OptionCode;
};

UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDPointEditorViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 SelectedActionId;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	EUDPointType SelectedType;
	// Fields.
	UPROPERTY(BlueprintReadOnly)
	FUDDealPointInfo CurrentPoint;
public:
	UFUNCTION(BlueprintCallable)
	void SetBindingTarget(FUDDealPointInfo info)
	{
		CurrentPoint = info;
		SetSelectedActionId(CurrentPoint.ActionId);
		SetSelectedType(CurrentPoint.Type);
		EditedDealUpdated.Broadcast();
	}

	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// HACK for forcing updating this.
			if (CurrentPoint.DealUniqueId != 0)
			{
				SetBindingTarget(CurrentPoint);
			}
		}
		else
		{
			// This should not be visible ?
		}
	}

	/**
	 * Invoked when this requires subviews to rebind.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDEditedDealUpdated EditedDealUpdated;

	UFUNCTION(BlueprintCallable)
	TArray<FUDNamedOption> GetAvailableActions()
	{
		return {
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "UNDEFINED")), 0),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Give Gift")), UUDUnconditionalGiftAction::ActionTypeId),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Transfer Tile")), UUDTransferTileAction::ActionTypeId),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Grant Exploit Tile")), UUDGrantExploitTilePermissionAction::ActionTypeId),
		};
	}

	UFUNCTION(BlueprintCallable)
	int32 GetSelectedActionCode()
	{
		return GetSelectedActionId();
	}
	UFUNCTION(BlueprintCallable)
	TArray<FUDNamedOption> GetAvailableTypes()
	{
		return {
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Propose")), UUDDefinePointTypeDealAction::PointTypeToInteger(EUDPointType::Proposal)),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Demand")), UUDDefinePointTypeDealAction::PointTypeToInteger(EUDPointType::Demand)),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Offer")), UUDDefinePointTypeDealAction::PointTypeToInteger(EUDPointType::Offer)),
		};
	}
	UFUNCTION(BlueprintCallable)
	int32 GetSelectedTypeCode()
	{
		return UUDDefinePointTypeDealAction::PointTypeToInteger(GetSelectedType());
	}
	UFUNCTION(BlueprintCallable)
	void UpdatePointAction(FUDNamedOption option)
	{
		if (option.OptionCode == 0)
			return;
		int32 actionId = option.OptionCode;
		ActionModel->RequestAction(
			ActionModel->UpdateActionDiscussionPointAction(
				CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, actionId));
	}
	UFUNCTION(BlueprintCallable)
	void UpdatePointType(FUDNamedOption option) 
	{
		EUDPointType type = UUDDefinePointTypeDealAction::IntegerToPointType(option.OptionCode);
		ActionModel->RequestAction(
			ActionModel->UpdateTypeDiscussionPointAction(
				CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, type));
	}

	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetAllPlayers()
	{
		return ActionModel->GetPlayerList();
	}
private:
	// MVVM Setters & Getters
	void SetSelectedActionId(int32 newSelectedActionId)
	{
		UE_MVVM_SET_PROPERTY_VALUE(SelectedActionId, newSelectedActionId);
	}
	int32 GetSelectedActionId() const
	{
		return SelectedActionId;
	}

	void SetSelectedType(EUDPointType newSelectedType)
	{
		UE_MVVM_SET_PROPERTY_VALUE(SelectedType, newSelectedType);
	}
	EUDPointType GetSelectedType() const
	{
		return SelectedType;
	}

};


UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDParticipantViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	 // MVVM Field.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Name;
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
	// MVVM Setters & Getters
	void SetName(FString newName)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Name, newName);
	}
	FString GetName() const
	{
		return Name;
	}
	void SetIsParticipant(bool newIsParticipant)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsParticipant, newIsParticipant);
	}
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
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString SessionDescription;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsSessionActive;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentDeal = 0;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsEnabledPreviousDeal = 0;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsEnabledNextDeal = 0;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsModerator = false;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	EUDDealSimulationState DealState = EUDDealSimulationState::Undefined;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	EUDDealSimulationResult DealResult = EUDDealSimulationResult::Undefined;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CurrentChatMessage;
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
	 * Invoked when this requires view to rebind.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDPDealPointUpdated PointsOnUpdated;
	/**
	 * Invoked when this requires view to rebind.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDChatUpdated ChatOnUpdated;

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
	UFUNCTION(BlueprintCallable)
	void ItemAddPoint()
	{
		ActionModel->RequestAction(ActionModel->CreateDiscussionPointAction(CurrentDealItem.DealUniqueId));
	}
	UFUNCTION(BlueprintCallable)
	void ItemAddMessage()
	{
		ActionModel->RequestAction(ActionModel->CreateChatMessageAction(CurrentDealItem.DealUniqueId, CurrentChatMessage));
		SetCurrentChatMessage(FString());
	}

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetRecentChatMessages()
	{
		return ActionModel->GetDealMessages(CurrentDealItem.DealUniqueId);
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
		SetDealState(info.State);
		SetDealResult(info.Result);
		SetIsModerator(false);
		auto rs1 = FText(
			LOCTEXT("DealState", "No deal is currently discussed with you.")
		).ToString();
		SetSessionDescription(rs1);
	}
	void OnDealPresent(FUDDealInfo info)
	{
		SetIsSessionActive(true);
		SetDealState(info.State);
		SetDealResult(info.Result);
		auto rs1 = FText::Format(
			LOCTEXT("DealState", "Deal is currently discussed with you, ID{0}."),
			info.DealUniqueId
		).ToString();
		SetSessionDescription(rs1);
		SetIsModerator(ActionModel->IsModerator(info.DealUniqueId));

		auto data = ActionModel->GetDealParticipants(info.DealUniqueId);
		ParticipantsOnUpdated.Broadcast(data);
		ChatOnUpdated.Broadcast();
		PointsOnUpdated.Broadcast(ActionModel->GetDealPointsTree(info.DealUniqueId));

	}
private:
	// MVVM Setters & Getters
	void SetSessionDescription(FString newSessionDescription)
	{
		UE_MVVM_SET_PROPERTY_VALUE(SessionDescription, newSessionDescription);
	}
	FString GetSessionDescription() const
	{
		return SessionDescription;
	}
	void SetIsSessionActive(bool newIsSessionActive)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsSessionActive, newIsSessionActive);
	}
	bool GetIsSessionActive() const
	{
		return IsSessionActive;
	}
	void SetCurrentDeal(int32 newCurrentDeal)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentDeal, newCurrentDeal);
	}
	int32 GetCurrentDeal() const
	{
		return CurrentDeal;
	}
	void SetIsEnabledPreviousDeal(bool newIsEnabledPreviousDeal)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsEnabledPreviousDeal, newIsEnabledPreviousDeal);
	}
	bool GetIsEnabledPreviousDeal() const
	{
		return IsEnabledPreviousDeal;
	}
	void SetIsEnabledNextDeal(bool newIsEnabledNextDeal)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsEnabledNextDeal, newIsEnabledNextDeal);
	}
	bool GetIsEnabledNextDeal() const
	{
		return IsEnabledNextDeal;
	}
	void SetDealState(EUDDealSimulationState newDealState)
	{
		UE_MVVM_SET_PROPERTY_VALUE(DealState, newDealState);
	}
	EUDDealSimulationState GetDealState() const
	{
		return DealState;
	}
	void SetDealResult(EUDDealSimulationResult newDealResult)
	{
		UE_MVVM_SET_PROPERTY_VALUE(DealResult, newDealResult);
	}
	EUDDealSimulationResult GetDealResult() const
	{
		return DealResult;
	}
	void SetIsModerator(bool newIsModerator)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsModerator, newIsModerator);
	}
	bool GetIsModerator() const
	{
		return IsModerator;
	}
	void SetCurrentChatMessage(FString newCurrentChatMessage)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentChatMessage, newCurrentChatMessage);
	}
	FString GetCurrentChatMessage() const
	{
		return CurrentChatMessage;
	}
};
#undef LOCTEXT_NAMESPACE