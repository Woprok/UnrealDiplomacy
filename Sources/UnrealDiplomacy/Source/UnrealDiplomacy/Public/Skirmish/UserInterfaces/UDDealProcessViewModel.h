// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/Actions/UDGameActionGiftIrrevocable.h"
#include "Core/Simulation/Actions/UDGameActionPermitTileExploit.h"
#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "Core/Simulation/Actions/UDGameActionGiftIrrevocable.h"
#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyType.h"

#include "Core/Simulation/UDActionData.h"

#include "Core/Simulation/Actions/UDDealAction.h"
#include "Core/Simulation/Actions/UDDealActionContractCreate.h"
#include "Core/Simulation/Actions/UDDealActionContractExecute.h"
#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"
#include "Core/Simulation/Actions/UDDealActionContractPointReject.h"
#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"
#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"
#include "Core/Simulation/Actions/UDDealActionCreate.h"
#include "Core/Simulation/Actions/UDDealActionFinalize.h"
#include "Core/Simulation/Actions/UDDealActionMessageSend.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteAccept.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteReject.h"
#include "Core/Simulation/Actions/UDDealActionParticipantKick.h"
#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"
#include "Core/Simulation/Actions/UDDealActionPointAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointIgnore.h"
#include "Core/Simulation/Actions/UDDealActionPointModify.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyAction.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerRemove.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyResetParameters.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTargetAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTargetRemove.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTile.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTileValue.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyType.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyValue.h"
#include "Core/Simulation/Actions/UDDealActionPointRemove.h"
#include "Core/Simulation/Actions/UDDealActionReady.h"
#include "Core/Simulation/Actions/UDDealActionReadyReset.h"
#include "Core/Simulation/Actions/UDDealActionReadyRevert.h"
#include "Core/Simulation/Actions/UDDealActionResultClose.h"
#include "Core/Simulation/Actions/UDDealActionResultDisassemble.h"
#include "Core/Simulation/Actions/UDDealActionResultPass.h"
#include "Core/Simulation/Actions/UDDealActionResultVeto.h"
#include "Core/Simulation/Actions/UDDealActionStateAssemble.h"
#include "Core/Simulation/Actions/UDDealActionStateExtendingDraft.h"
#include "Core/Simulation/Actions/UDDealActionVoteNo.h"
#include "Core/Simulation/Actions/UDDealActionVoteYes.h"

#include "UDDealProcessViewModel.generated.h"

#define LOCTEXT_NAMESPACE "ActionUI"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDParticipantsUpdated, FUDDealParticipantsInfo, infos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDPDealPointUpdated, FUDDealPointTreeInfo, dealRoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDChatUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDEditedDealUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDActionInfoUpdated);

UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointActionFinalViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	FString Description;
	// Resolved actions can't be updated by owner.
	bool IsResolved;
	// Sabotaged actions can't be sabotaged again.
	bool IsSabotaged;
	// Actions are different based on perspective.
	bool IsExecutor;
	// Fields.
	FUDDealActionInfo CurrentData;
public:
	UFUNCTION(BlueprintCallable)
	void SetBindingTarget(FUDDealActionInfo action, bool isOwner)
	{
		CurrentData = action;
		FUDActionData data = action.ActionBody.Action;
		TStringBuilder<64> desc;
		desc.Append(FText::Format(LOCTEXT("ActionPreview", "Action: {0}. "),
			data.ActionTypeId).ToString()
		);
		desc.Append(FText::Format(LOCTEXT("ActionPreview", "Invoked by: {0}. "),
			data.InvokerFactionId).ToString()

		);
		desc.Append(FText(LOCTEXT("ActionPreview", "Text param: ")).ToString());
		desc.Append(data.TextParameter);
		desc.Append(FText(LOCTEXT("ActionPreview", ".\n")).ToString());

		desc.Append(FText::Format(LOCTEXT("ActionPreview", "Value param count: {0} Params:\n"),
			data.ValueParameters.Num()).ToString()
		);
		for (auto param : data.ValueParameters)
		{
			desc.Append(FText::Format(LOCTEXT("ActionPreview", "Prm: {0}. "),
				param).ToString()
			);
		}
		//SetDescription(desc.ToString());
		//SetIsResolved(action.ActionBody.SelectedResult != EUDDealActionResult::Unresolved);
		//SetIsSabotaged(action.ActionBody.WasSabotaged);
		//SetIsExecutor(isOwner);
	}

	UFUNCTION(BlueprintCallable)
	void AcceptAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionContractPointAccept::ActionTypeId,
				{ CurrentData.DealUniqueId, CurrentData.ActionIndex }));
	}

	UFUNCTION(BlueprintCallable)
	void ChangeAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionContractPointTamper::ActionTypeId,
				{ CurrentData.DealUniqueId, CurrentData.ActionIndex }));
	}

	UFUNCTION(BlueprintCallable)
	void DenyAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionContractPointReject::ActionTypeId,
				{ CurrentData.DealUniqueId, CurrentData.ActionIndex }));
	}

	UFUNCTION(BlueprintCallable)
	void SabotageAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionContractPointSabotage::ActionTypeId,
				{ CurrentData.DealUniqueId, CurrentData.ActionIndex }));
	}
};

UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointActionPreviewViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	FString Description;
	// Fields.
public:
	UFUNCTION(BlueprintCallable)
		void SetBindingTarget(FUDActionData action)
	{
		TStringBuilder<64> desc;
		desc.Append(FText::Format(LOCTEXT("ActionPreview", "Action: {0}. "),
			action.ActionTypeId).ToString()
		);
		desc.Append(FText::Format(LOCTEXT("ActionPreview", "Invoked by: {0}. "),
			action.InvokerFactionId).ToString()

		);
		desc.Append(FText(LOCTEXT("ActionPreview", "Text param: ")).ToString());
		desc.Append(action.TextParameter);
		desc.Append(FText(LOCTEXT("ActionPreview", ".\n")).ToString());

		desc.Append(FText::Format(LOCTEXT("ActionPreview", "Value param count: {0} Params:\n"), 
			action.ValueParameters.Num()).ToString()
		);
		for (auto param : action.ValueParameters)
		{
			desc.Append(FText::Format(LOCTEXT("ActionPreview", "Prm: {0}. "), 
				param).ToString()
			);
		}
		//SetDescription(desc.ToString());
	}
};

UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointParticipantViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	FString Name;
	bool IsInvoker;
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
		//SetName(rs1);
		bool isInv = ActionModel->IsDealPointInvoker(CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id);
		//SetIsInvoker(isInv);
		bool isTar = ActionModel->IsDealPointTarget(CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id);
		//SetIsTarget(isTar);

	}
	UFUNCTION(BlueprintCallable)
	void AddToInvokers()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyInvokerAdd::ActionTypeId,
				{ CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id }));
	}
	UFUNCTION(BlueprintCallable)
	void RemoveFromInvokers()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyInvokerRemove::ActionTypeId,
				{ CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id }));
	}
	UFUNCTION(BlueprintCallable)
	void AddToTargets()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyTargetAdd::ActionTypeId,
				{ CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id }));
	}
	UFUNCTION(BlueprintCallable)
	void RemoveFromTargets()
	{
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyTargetRemove::ActionTypeId,
				{ CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, CurrentPlayer.Id }));
	}
};


UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	FString Title;
	FString Description;
	FString InvokerList;
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
			LOCTEXT("Participant", "Discussed item {0} for deal {1}"),
			info.PointUniqueId,
			info.DealUniqueId
		).ToString();
		//SetTitle(rs1);
		auto rs2 = FText::Format(
			LOCTEXT("Participant", "Type {0}: Action {1} will be applied to all targets."),
			(int32)CurrentPoint.Type,
			CurrentPoint.ActionId
		).ToString();
		//SetDescription(rs2);
		
		auto invokers = ActionModel->GetInvokerPointPlayerList(info.DealUniqueId, info.PointUniqueId);
		TStringBuilder<64> invList;
		for (auto player : invokers)
		{
			invList.Append(FString::FromInt(player.Id));
			invList.Append(",");
		}
		//SetInvokerList(invList.ToString());

		auto targets = ActionModel->GetTargetPointPlayerList(info.DealUniqueId, info.PointUniqueId);
		TStringBuilder<64> tarList;
		for (auto player : targets)
		{
			tarList.Append(FString::FromInt(player.Id));
			tarList.Append(",");
		}
		//SetTargetList(tarList.ToString());
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
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionPointChildAdd::ActionTypeId,{ CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId }));
	}
};

/**
 * Each parameter type that is supported for actions
 * TODO Make this modular.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDActionParameterEditorViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	int32 ValueParameter;
	FIntPoint TileParameter;
	FString TextParameter;
	bool HasValueParameter;
	bool HasTileParameter;
	bool HasTextParameter;
	//EUDParameterCountType InvokeType;
public:
	//UFUNCTION(BlueprintCallable)
//	void UpdateSelection(EUDParameterCountType inInvokeType)
	//{
		//InvokeType = inInvokeType;
		//switch (inInvokeType)
		//{
		//case EUDParameterCountType::None:
		//	SetHasTileParameter(false);
		//	SetHasValueParameter(false);
		//	SetHasTextParameter(false);
		//	break;
		//case EUDParameterCountType::SingleValue:
		//	SetHasTileParameter(false);
		//	SetHasValueParameter(true);
		//	SetHasTextParameter(false);
		//	break;
		//case EUDParameterCountType::SingleTile:
		//	SetHasTileParameter(true);
		//	SetHasValueParameter(false);
		//	SetHasTextParameter(false);
		//	break;
		//case EUDParameterCountType::TileValue:
		//	SetHasTileParameter(true);
		//	SetHasValueParameter(true);
		//	SetHasTextParameter(false);
		//	break;
		//default:
		//	SetHasTileParameter(false);
		//	SetHasValueParameter(false);
		//	SetHasTextParameter(false);
		//	break;
		//}
	//}
};

// TODO make this modular
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointEditorViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	int32 SelectedActionId;
	EUDPointType SelectedType;
	// Fields.
	FUDDealPointInfo CurrentPoint;
public:
	UFUNCTION(BlueprintCallable)
	void SetBindingTarget(FUDDealPointInfo info)
	{
		CurrentPoint = info;
		//SetSelectedActionId(CurrentPoint.ActionId);
		//SetSelectedType(CurrentPoint.Type);
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
	}

	/**
	 * Invoked when this requires subviews to rebind.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDEditedDealUpdated EditedDealUpdated;

	/*/TArray<FUDNamedOption> GetAvailableTypes()
	{
		return {
			//FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Propose")), UUDDealActionPointModifyType::PointTypeToInteger(EUDPointType::Proposal)),
			//FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Demand")), UUDDealActionPointModifyType::PointTypeToInteger(EUDPointType::Demand)),
			//FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Offer")), UUDDealActionPointModifyType::PointTypeToInteger(EUDPointType::Offer)),
		};
	}*//*/
	UFUNCTION(BlueprintCallable)
	void UpdatePointAction(FUDNamedOption option)
	{
		if (option.OptionCode == 0)
			return;
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		int32 actionId = option.OptionCode;
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyAction::ActionTypeId,
				{ dealId, pointId, actionId }));
		// TODO add dynamic editable parameters
		// TODO remove this before finishing
		// HACK: please do not crash
		// forces parameter to value based on predefined options for testing
		switch (actionId)
		{
		case UUDGameActionGiftIrrevocable::ActionTypeId:
			ActionModel->RequestAction(
				ActionModel->GetAction(UUDDealActionPointModifyValue::ActionTypeId,
					{ dealId, pointId, 42 }));
			break;
		case UUDGameActionTileTransfer::ActionTypeId:
			ActionModel->RequestAction(
				ActionModel->GetAction(UUDDealActionPointModifyTile::ActionTypeId,
					{ dealId, pointId, 3, 3 }));
			break;
		case UUDGameActionPermitTileExploit::ActionTypeId:
			ActionModel->RequestAction(
				ActionModel->GetAction(UUDDealActionPointModifyTileValue::ActionTypeId,
					{ dealId, pointId, 2, 2 }));
			break;
		default:
			// safely ignore the fake actions or actions that don't need parameter.
			break;
		}
	}*/
	UFUNCTION(BlueprintCallable)
	void ItemUpdateValue(int32 value)
	{
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyValue::ActionTypeId,
				{ dealId, pointId, value }));
	}
	UFUNCTION(BlueprintCallable)
	void ItemUpdateTile(FIntPoint tile)
	{
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyTile::ActionTypeId,
				{ dealId, pointId, tile.X, tile.Y }));
	}
	UFUNCTION(BlueprintCallable)
	void ItemUpdateTileValue(FIntPoint tile, int32 value)
	{
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyTileValue::ActionTypeId,
				{ dealId, pointId, tile.X, tile.Y, value }));
	}

	/*/UFUNCTION(BlueprintCallable)
	void UpdatePointType(FUDNamedOption option) 
	{
		EUDPointType type = UUDDealActionPointModifyType::IntegerToPointType(option.OptionCode);
		int32 typeAsInt = option.OptionCode; // WUT
		ActionModel->RequestAction(
			ActionModel->GetAction(UUDDealActionPointModifyType::ActionTypeId,
				{ CurrentPoint.DealUniqueId, CurrentPoint.PointUniqueId, typeAsInt }));
	}*/

	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetAllPlayers()
	{
		return ActionModel->GetPlayerList();
	}
};


UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDParticipantViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	 // MVVM Field.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Name;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsParticipant;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsReady;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsPositiveVote;
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
		SetIsReady(ActionModel->IsReadyInCurrentDeal(dealUniqueId, info.Id));
		SetIsPositiveVote(ActionModel->IsPositiveVotingCurrentDeal(dealUniqueId, info.Id));
	}

	UFUNCTION(BlueprintCallable)
	void InvitePlayer()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionParticipantInvite::ActionTypeId, { CurrentDealUniqueId, CurrentInfo.Id }));
	}
	UFUNCTION(BlueprintCallable)
	void LeaveDeal()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionParticipantLeave::ActionTypeId, { CurrentDealUniqueId, CurrentInfo.Id }));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToReady()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionReadyRevert::ActionTypeId, { CurrentDealUniqueId }));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToNotReady()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionReady::ActionTypeId, { CurrentDealUniqueId }));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToYes()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionVoteYes::ActionTypeId, { CurrentDealUniqueId }));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToNo()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionVoteNo::ActionTypeId, { CurrentDealUniqueId }));
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
	void SetIsReady(bool newIsReady)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsReady, newIsReady);
	}
	bool GetIsReady() const
	{
		return IsReady;
	}
	void SetIsPositiveVote(bool newIsPositiveVote)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsPositiveVote, newIsPositiveVote);
	}
	bool GetIsPositiveVote() const
	{
		return IsPositiveVote;
	}
};

UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealProcessViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Fields
	FString SessionDescription;
	bool IsSessionActive;
	int32 CurrentDeal = 0;
	bool IsModerator = false;
	EUDDealSimulationState DealState = EUDDealSimulationState::Undefined;
	EUDDealSimulationResult DealResult = EUDDealSimulationResult::Undefined;
	int32 CurrentReady;
	int32 MaxReady;
	int32 CurrentPositiveVote;
	int32 MaxVote;
protected:
	TArray<int32> DealsByTimeline;
	//FUDDealInfo CurrentDealItem;
	int32 CurrentIndex;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			TArray<int32> deals = ActionModel->GetDealIds();
		}
	}
	FUDParticipantsUpdated ParticipantsOnUpdated;
	FUDPDealPointUpdated PointsOnUpdated;
	FUDChatUpdated ChatOnUpdated;
	FUDActionInfoUpdated ActionInfoUpdated;

	UFUNCTION(BlueprintCallable)
	void ItemForceResolution()
	{
		//ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionContractCreate::ActionTypeId,{ CurrentDealItem.DealUniqueId }));
	}
	UFUNCTION(BlueprintCallable)
	void ItemForceExecution()
	{
		//ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionContractExecute::ActionTypeId,{ CurrentDealItem.DealUniqueId }));
	}
	//UFUNCTION(BlueprintCallable)
	//TArray<FUDDealActionInfo> GetAllActionForCurrentPlayer()
	//{
	//	//return ActionModel->GetAllActionForCurrentPlayer(CurrentDealItem.DealUniqueId);
	//}
	//UFUNCTION(BlueprintCallable)
	//TArray<FUDDealActionInfo> GetAllActionForOtherPlayers()
	//{
	//	//return ActionModel->GetAllActionForOtherPlayers(CurrentDealItem.DealUniqueId);
	//}
	//
	//UFUNCTION(BlueprintCallable)
	//TArray<FUDActionData> GetCurrentActionPreviewList()
	//{
	//	//return ActionModel->GetDealPointsAsUnfoldedActions(CurrentDealItem.DealUniqueId);
	//}
protected:
	void UpdateView(int32 dealHistoryCount, int32 dealUniqueId, int32 arrayPosition)
	{
		//CurrentDealItem = ActionModel->GetDealInfo(dealUniqueId);
		CurrentDeal = dealUniqueId;
		CurrentIndex = arrayPosition;

		//OnDealPresent(CurrentDealItem);
	}
	/*/
	void OnDealEmpty(FUDDealInfo info)
	{
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
		SetCurrentReady(ActionModel->GetReadyParticipantCount(info.DealUniqueId));
		SetMaxReady(ActionModel->GetParticipantCount(info.DealUniqueId));
		SetCurrentPositiveVote(ActionModel->GetPositiveVoteCurrentDealCount(info.DealUniqueId));
		SetMaxVote(ActionModel->GetParticipantCount(info.DealUniqueId));

		auto data = ActionModel->GetDealParticipants(info.DealUniqueId);
		ParticipantsOnUpdated.Broadcast(data);
		ChatOnUpdated.Broadcast();
		PointsOnUpdated.Broadcast(ActionModel->GetDealPointsTree(info.DealUniqueId));
		ActionInfoUpdated.Broadcast();
	}*/	
};
#undef LOCTEXT_NAMESPACE