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
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Description;
	// Resolved actions can't be updated by owner.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsResolved;
	// Sabotaged actions can't be sabotaged again.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsSabotaged;
	// Actions are different based on perspective.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
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
			data.InvokerPlayerId).ToString()

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
		SetDescription(desc.ToString());
		SetIsResolved(action.ActionBody.SelectedResult != EUDDealActionResult::Unresolved);
		SetIsSabotaged(action.ActionBody.WasSabotaged);
		SetIsExecutor(isOwner);
	}

	UFUNCTION(BlueprintCallable)
	void AcceptAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetAcceptFinalItemDealAction(
				CurrentData.DealUniqueId, CurrentData.ActionIndex));
	}

	UFUNCTION(BlueprintCallable)
	void ChangeAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetAlterFinalItemDealAction(
				CurrentData.DealUniqueId, CurrentData.ActionIndex));
	}

	UFUNCTION(BlueprintCallable)
	void DenyAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetDenyFinalItemDealAction(
				CurrentData.DealUniqueId, CurrentData.ActionIndex));
	}

	UFUNCTION(BlueprintCallable)
	void SabotageAction()
	{
		ActionModel->RequestAction(
			ActionModel->GetSabotageFinalItemDealAction(
				CurrentData.DealUniqueId, CurrentData.ActionIndex));
	}
private:
	// MVVM Setters & Getters
	void SetDescription(FString newDescription)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Description, newDescription);
	}
	FString GetDescription() const
	{
		return Description;
	}
	void SetIsResolved(bool newIsResolved)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsResolved, newIsResolved);
	}
	bool GetIsResolved() const
	{
		return IsResolved;
	}
	void SetIsSabotaged(bool newIsSabotaged)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsSabotaged, newIsSabotaged);
	}
	bool GetIsSabotaged() const
	{
		return IsSabotaged;
	}
	void SetIsExecutor(bool newIsExecutor)
	{
		UE_MVVM_SET_PROPERTY_VALUE(IsExecutor, newIsExecutor);
	}
	bool GetIsExecutor() const
	{
		return IsExecutor;
	}
};

UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointActionPreviewViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	// MVVM Field.
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
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
			action.InvokerPlayerId).ToString()

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
		SetDescription(desc.ToString());
	}
private:
	// MVVM Setters & Getters
	void SetDescription(FString newDescription)
	{
		UE_MVVM_SET_PROPERTY_VALUE(Description, newDescription);
	}
	FString GetDescription() const
	{
		return Description;
	}
};

UCLASS(Blueprintable, BlueprintType)
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


UCLASS(Blueprintable, BlueprintType)
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
			LOCTEXT("Participant", "Discussed item {0} for deal {1}"),
			info.PointUniqueId,
			info.DealUniqueId
		).ToString();
		SetTitle(rs1);
		auto rs2 = FText::Format(
			LOCTEXT("Participant", "Type {0}: Action {1} will be applied to all targets."),
			(int32)CurrentPoint.Type,
			CurrentPoint.ActionId
		).ToString();
		SetDescription(rs2);

		auto invokers = ActionModel->GetInvokerPointPlayerList(info.DealUniqueId, info.PointUniqueId);
		TStringBuilder<64> invList;
		for (auto player : invokers)
		{
			invList.Append(FString::FromInt(player.Id));
			invList.Append(",");
		}
		SetInvokerList(invList.ToString());

		auto targets = ActionModel->GetTargetPointPlayerList(info.DealUniqueId, info.PointUniqueId);
		TStringBuilder<64> tarList;
		for (auto player : targets)
		{
			tarList.Append(FString::FromInt(player.Id));
			tarList.Append(",");
		}
		SetTargetList(tarList.ToString());
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

USTRUCT(Blueprintable, BlueprintType)
struct FUDNamedOption
{
	GENERATED_BODY()
public:
	FUDNamedOption() {}
	FUDNamedOption(FText name, int32 optionCode) 
		: Name(name), OptionCode(optionCode) {}
	FUDNamedOption(FText name, int32 optionCode, int32 optionCode2)
		: Name(name), OptionCode(optionCode), OptionCode2(optionCode2) {}
	UPROPERTY(BlueprintReadOnly)
	FText Name;
	UPROPERTY(BlueprintReadOnly)
	int32 OptionCode;
	UPROPERTY(BlueprintReadOnly)
	int32 OptionCode2;
};

/**
 * Represents parameter type
 */
UENUM(BlueprintType)
enum class EUDParameterCountType : uint8
{
	None = 0,
	SingleValue = 1,
	SingleTile = 2,
	TileValue = 3,
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
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 ValueParameter;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FIntPoint TileParameter;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString TextParameter;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasValueParameter;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasTileParameter;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool HasTextParameter;
	UPROPERTY(BlueprintReadOnly)
	EUDParameterCountType InvokeType;
public:
	UFUNCTION(BlueprintCallable)
	void UpdateSelection(EUDParameterCountType inInvokeType)
	{
		InvokeType = inInvokeType;
		switch (inInvokeType)
		{
		case EUDParameterCountType::None:
			SetHasTileParameter(false);
			SetHasValueParameter(false);
			SetHasTextParameter(false);
			break;
		case EUDParameterCountType::SingleValue:
			SetHasTileParameter(false);
			SetHasValueParameter(true);
			SetHasTextParameter(false);
			break;
		case EUDParameterCountType::SingleTile:
			SetHasTileParameter(true);
			SetHasValueParameter(false);
			SetHasTextParameter(false);
			break;
		case EUDParameterCountType::TileValue:
			SetHasTileParameter(true);
			SetHasValueParameter(true);
			SetHasTextParameter(false);
			break;
		default:
			SetHasTileParameter(false);
			SetHasValueParameter(false);
			SetHasTextParameter(false);
			break;
		}
	}
	UFUNCTION(BlueprintCallable)
	void UpdateParameters(FIntPoint tile, int32 value)
	{
		switch (InvokeType)
		{
		case EUDParameterCountType::None:
			break;
		case EUDParameterCountType::SingleValue:
			SetValueParameter(value);
			break;
		case EUDParameterCountType::SingleTile:
			SetTileParameter(tile);
			break;
		case EUDParameterCountType::TileValue:
			SetTileParameter(tile);
			SetValueParameter(value);
			break;
		default:
			break;
		}
	}
private:
	// MVVM Setters & Getters
	void SetValueParameter(int32 newValueParameter)
	{
		UE_MVVM_SET_PROPERTY_VALUE(ValueParameter, newValueParameter);
	}
	int32 GetValueParameter() const
	{
		return ValueParameter;
	}
	void SetTileParameter(FIntPoint newTileParameter)
	{
		UE_MVVM_SET_PROPERTY_VALUE(TileParameter, newTileParameter);
	}
	FIntPoint GetTileParameter() const
	{
		return TileParameter;
	}
	void SetTextParameter(FString newTextParameter)
	{
		UE_MVVM_SET_PROPERTY_VALUE(TextParameter, newTextParameter);
	}
	FString GetTextParameter() const
	{
		return TextParameter;
	}
	void SetHasValueParameter(bool newHasValueParameter)
	{
		UE_MVVM_SET_PROPERTY_VALUE(HasValueParameter, newHasValueParameter);
	}
	bool GetHasValueParameter() const
	{
		return HasValueParameter;
	}
	void SetHasTileParameter(bool newHasTileParameter)
	{
		UE_MVVM_SET_PROPERTY_VALUE(HasTileParameter, newHasTileParameter);
	}
	bool GetHasTileParameter() const
	{
		return HasTileParameter;
	}
	void SetHasTextParameter(bool newHasTextParameter)
	{
		UE_MVVM_SET_PROPERTY_VALUE(HasTextParameter, newHasTextParameter);
	}
	bool GetHasTextParameter() const
	{
		return HasTextParameter;
	}
};

// TODO make this modular
UCLASS(Blueprintable, BlueprintType)
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
	TArray<FUDNamedOption> GetAvailableTiles()
	{
		return {
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "-x-")), -1, -1),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "0x0")), 0, 0),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "0x1")), 0, 1),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "0x2")), 0, 2),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "1x0")), 1, 0),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "1x1")), 1, 1),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "1x2")), 1, 2),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "2x0")), 2, 0),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "2x1")), 2, 1),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "2x2")), 2, 2),
		};
	}

	UFUNCTION(BlueprintCallable)
	EUDParameterCountType GetCurrentForAction()
	{
		switch (CurrentPoint.ActionId)
		{
		case UUDGameActionGiftIrrevocable::ActionTypeId:
			return EUDParameterCountType::SingleValue;
		case UUDGameActionTileTransfer::ActionTypeId:
			return EUDParameterCountType::TileValue;
		case UUDGameActionPermitTileExploit::ActionTypeId:
			return EUDParameterCountType::TileValue;
		case UUDGameActionThroneAbdicate::ActionTypeId:
			return EUDParameterCountType::None;
		default:
			return EUDParameterCountType::None;
			break;
		}
	}

	UFUNCTION(BlueprintCallable)
	int32 GetSelectedValue()
	{
		if (CurrentPoint.ValueParameters.Num() == 1)
			return CurrentPoint.ValueParameters[0];
		if (CurrentPoint.ValueParameters.Num() == 3)
			return CurrentPoint.ValueParameters[2];
		return 0;
	}

	UFUNCTION(BlueprintCallable)
	FIntPoint GetSelectedTile()
	{
		if (CurrentPoint.ValueParameters.Num() >= 2)
			return FIntPoint(CurrentPoint.ValueParameters[0], CurrentPoint.ValueParameters[1]);
		return FIntPoint(-1, -1);
	}

	UFUNCTION(BlueprintCallable)
	TArray<FUDNamedOption> GetAvailableActions()
	{
		return {
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "UNDEFINED")), 0),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Give Gift")), UUDGameActionGiftIrrevocable::ActionTypeId),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Transfer Tile")), UUDGameActionTileTransfer::ActionTypeId),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Grant Exploit Tile")), UUDGameActionPermitTileExploit::ActionTypeId),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Abdicate")), UUDGameActionThroneAbdicate::ActionTypeId),
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
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Propose")), UUDDealActionPointModifyType::PointTypeToInteger(EUDPointType::Proposal)),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Demand")), UUDDealActionPointModifyType::PointTypeToInteger(EUDPointType::Demand)),
			FUDNamedOption(FText(LOCTEXT("NamedActionOption", "Offer")), UUDDealActionPointModifyType::PointTypeToInteger(EUDPointType::Offer)),
		};
	}
	UFUNCTION(BlueprintCallable)
	int32 GetSelectedTypeCode()
	{
		return UUDDealActionPointModifyType::PointTypeToInteger(GetSelectedType());
	}
	UFUNCTION(BlueprintCallable)
	void UpdatePointAction(FUDNamedOption option)
	{
		if (option.OptionCode == 0)
			return;
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		int32 actionId = option.OptionCode;
		ActionModel->RequestAction(
			ActionModel->UpdateActionDiscussionPointAction(
				dealId, pointId, actionId));
		// TODO add dynamic editable parameters
		// TODO remove this before finishing
		// HACK: please do not crash
		// forces parameter to value based on predefined options for testing
		switch (actionId)
		{
		case UUDGameActionGiftIrrevocable::ActionTypeId:
			ActionModel->RequestAction(
				ActionModel->UpdateChangeValueParameterDiscussionPointAction(
					dealId, pointId, 42));
			break;
		case UUDGameActionTileTransfer::ActionTypeId:
			ActionModel->RequestAction(
				ActionModel->UpdateChangeTileParameterDiscussionPointAction(
					dealId, pointId, 3, 3));
			break;
		case UUDGameActionPermitTileExploit::ActionTypeId:
			ActionModel->RequestAction(
				ActionModel->UpdateChangeTileParameterDiscussionPointAction(
					dealId, pointId, 2, 2));
			break;
		default:
			// safely ignore the fake actions or actions that don't need parameter.
			break;
		}
	}
	UFUNCTION(BlueprintCallable)
	void ItemUpdateValue(int32 value)
	{
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		ActionModel->RequestAction(
			ActionModel->UpdateChangeValueParameterDiscussionPointAction(
				dealId, pointId, value));
	}
	UFUNCTION(BlueprintCallable)
	void ItemUpdateTile(FIntPoint tile)
	{
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		ActionModel->RequestAction(
			ActionModel->UpdateChangeTileParameterDiscussionPointAction(
				dealId, pointId, tile.X, tile.Y));
	}
	UFUNCTION(BlueprintCallable)
	void ItemUpdateTileValue(FIntPoint tile, int32 value)
	{
		int32 dealId = CurrentPoint.DealUniqueId;
		int32 pointId = CurrentPoint.PointUniqueId;
		ActionModel->RequestAction(
			ActionModel->UpdateChangeTileValueParameterDiscussionPointAction(
				dealId, pointId, tile.X, tile.Y, value));
	}

	UFUNCTION(BlueprintCallable)
	void UpdatePointType(FUDNamedOption option) 
	{
		EUDPointType type = UUDDealActionPointModifyType::IntegerToPointType(option.OptionCode);
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
		ActionModel->RequestAction(ActionModel->GetInviteParticipantDealAction(CurrentDealUniqueId, CurrentInfo.Id));
	}
	UFUNCTION(BlueprintCallable)
	void LeaveDeal()
	{
		ActionModel->RequestAction(ActionModel->GetLeaveParticipantDealAction(CurrentDealUniqueId, CurrentInfo.Id));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToReady()
	{
		ActionModel->RequestAction(ActionModel->GetReadyDealAction(CurrentDealUniqueId));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToNotReady()
	{
		ActionModel->RequestAction(ActionModel->GetNotReadyDealAction(CurrentDealUniqueId));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToYes()
	{
		ActionModel->RequestAction(ActionModel->GetPositiveVoteDealAction(CurrentDealUniqueId));
	}
	UFUNCTION(BlueprintCallable)
	void ChangeToNo()
	{
		ActionModel->RequestAction(ActionModel->GetNegativeVoteDealAction(CurrentDealUniqueId));
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

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
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
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentReady;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 MaxReady;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 CurrentPositiveVote;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 MaxVote;
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
	 * Invoked when this requires view to rebind.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDActionInfoUpdated ActionInfoUpdated;

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
	void ItemForceResolution()
	{
		ActionModel->RequestAction(ActionModel->GetFinalizeItemsDealAction(CurrentDealItem.DealUniqueId));
	}
	UFUNCTION(BlueprintCallable)
	void ItemForceExecution()
	{
		ActionModel->RequestAction(ActionModel->GetExecuteAllActionsDealAction(CurrentDealItem.DealUniqueId));
	}
	UFUNCTION(BlueprintCallable)
	TArray<FUDDealActionInfo> GetAllActionForCurrentPlayer()
	{
		return ActionModel->GetAllActionForCurrentPlayer(CurrentDealItem.DealUniqueId);
	}
	UFUNCTION(BlueprintCallable)
	TArray<FUDDealActionInfo> GetAllActionForOtherPlayers()
	{
		return ActionModel->GetAllActionForOtherPlayers(CurrentDealItem.DealUniqueId);
	}

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetRecentChatMessages()
	{
		return ActionModel->GetDealMessages(CurrentDealItem.DealUniqueId);
	}

	UFUNCTION(BlueprintCallable)
	TArray<FUDActionData> GetCurrentActionPreviewList()
	{
		return ActionModel->GetDealPointsAsUnfoldedActions(CurrentDealItem.DealUniqueId);
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
			action.ActionTypeId,
			action.InvokerPlayerId,
			action.ValueParameter
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
		SetCurrentReady(ActionModel->GetReadyParticipantCount(info.DealUniqueId));
		SetMaxReady(ActionModel->GetParticipantCount(info.DealUniqueId));
		SetCurrentPositiveVote(ActionModel->GetPositiveVoteCurrentDealCount(info.DealUniqueId));
		SetMaxVote(ActionModel->GetParticipantCount(info.DealUniqueId));

		auto data = ActionModel->GetDealParticipants(info.DealUniqueId);
		ParticipantsOnUpdated.Broadcast(data);
		ChatOnUpdated.Broadcast();
		PointsOnUpdated.Broadcast(ActionModel->GetDealPointsTree(info.DealUniqueId));
		ActionInfoUpdated.Broadcast();
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
	void SetCurrentReady(int32 newCurrentReady)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentReady, newCurrentReady);
	}
	int32 GetCurrentReady() const
	{
		return CurrentReady;
	}
	void SetMaxReady(int32 newMaxReady)
	{
		UE_MVVM_SET_PROPERTY_VALUE(MaxReady, newMaxReady);
	}
	int32 GetMaxReady() const
	{
		return MaxReady;
	}
	void SetCurrentPositiveVote(int32 newCurrentPositiveVote)
	{
		UE_MVVM_SET_PROPERTY_VALUE(CurrentPositiveVote, newCurrentPositiveVote);
	}
	int32 GetCurrentPositiveVote() const
	{
		return CurrentPositiveVote;
	}
	void SetMaxVote(int32 newMaxVote)
	{
		UE_MVVM_SET_PROPERTY_VALUE(MaxVote, newMaxVote);
	}
	int32 GetMaxVote() const
	{
		return MaxVote;
	}
	
};
#undef LOCTEXT_NAMESPACE