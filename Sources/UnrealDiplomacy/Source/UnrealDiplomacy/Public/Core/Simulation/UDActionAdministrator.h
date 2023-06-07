// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionInterface.h"
#include "UDWorldState.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"

// Forward Declarations
class IUDActionInterface;
class UUDWorldState;
class UUDActionManager;
class UUDActionDatabase;

#include "Core/Simulation/Actions/UDDealActionContractCreate.h"

#include "UDActionAdministrator.generated.h"




USTRUCT(BlueprintType)
struct FUDTurnInfo
{
	GENERATED_BODY()
public:
	FUDTurnInfo() {}
	FUDTurnInfo(int32 turn, int32 player) : Turn(turn), Player(player) { }
	UPROPERTY(BlueprintReadOnly)
	int32 Turn = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Player = 0;
};

USTRUCT(BlueprintType)
struct FUDPlayerInfo
{
	GENERATED_BODY()
public:
	FUDPlayerInfo() {}
	FUDPlayerInfo(int32 id) : Id(id) { }
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FText Name = FText::GetEmpty();
};

USTRUCT(BlueprintType)
struct FUDNationInfo
{
	GENERATED_BODY()
public:
	FUDNationInfo() {}
	FUDNationInfo(int32 id, int32 gold) : Id(id), Gold(gold) {}
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Gold = 0;
};

USTRUCT(BlueprintType)
struct FUDTileInfo
{
	GENERATED_BODY()
public:
	FUDTileInfo() {}
	FUDTileInfo(int32 owner, FIntPoint position, int32 type) : Owner(owner), Position(position), Type(type) {}
	UPROPERTY(BlueprintReadOnly)
	int32 Owner = 0;
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Position = FIntPoint(-1,-1);
	UPROPERTY(BlueprintReadOnly)
	int32 Type = 0;
};

USTRUCT(BlueprintType)
struct FUDDealParticipantsInfo
{
	GENERATED_BODY()
public:
	FUDDealParticipantsInfo() {}
	FUDDealParticipantsInfo(int32 dealUniqueId, TArray<FUDPlayerInfo> active, TArray<FUDPlayerInfo> blocked, TArray<FUDPlayerInfo> available) 
		: DealUniqueId(dealUniqueId),
		ActiveParticipants(active), BlockedParticipants(blocked), AvailableParticipants(available) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDPlayerInfo> ActiveParticipants;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDPlayerInfo> BlockedParticipants;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDPlayerInfo> AvailableParticipants;
};

USTRUCT(BlueprintType)
struct FUDDealInfo
{
	GENERATED_BODY()
public:
	FUDDealInfo() {}
	FUDDealInfo(int32 dealUniqueId, EUDDealSimulationState state, EUDDealSimulationResult result) 
		: DealUniqueId(dealUniqueId), State(state), Result(result) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	EUDDealSimulationState State;
	UPROPERTY(BlueprintReadOnly)
	EUDDealSimulationResult Result;
};

USTRUCT(BlueprintType)
struct FUDDealPointChildInfo
{
	GENERATED_BODY()
public:
	FUDDealPointChildInfo() {}
	FUDDealPointChildInfo(int32 dealUniqueId, int32 pointUniqueId)
		: DealUniqueId(dealUniqueId), PointUniqueId(pointUniqueId) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointUniqueId = 0;
};

USTRUCT(BlueprintType)
struct FUDDealPointTreeInfo
{
	GENERATED_BODY()
public:
	FUDDealPointTreeInfo() {}
	FUDDealPointTreeInfo(int32 dealUniqueId, int32 pointCount, TArray<FUDDealPointChildInfo> childPoints)
		: DealUniqueId(dealUniqueId), TotalPointCount(pointCount), PrimaryPoints(childPoints) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 TotalPointCount = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDDealPointChildInfo> PrimaryPoints;
};

USTRUCT(BlueprintType)
struct FUDDealPointInfo
{
	GENERATED_BODY()
public:
	FUDDealPointInfo() {}
	FUDDealPointInfo(int32 dealUniqueId, int32 pointUniqueId, EUDPointType type, int32 actionId) 
		: DealUniqueId(dealUniqueId), PointUniqueId(pointUniqueId), Type(type), ActionId(actionId) {}
	FUDDealPointInfo(int32 dealUniqueId, int32 pointUniqueId, EUDPointType type, int32 actionId,
		TArray<int32> valueParameters, FString textParameter)
		: DealUniqueId(dealUniqueId), PointUniqueId(pointUniqueId), Type(type), ActionId(actionId),
		ValueParameters(valueParameters), TextParameter(textParameter) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointUniqueId = 0;
	UPROPERTY(BlueprintReadOnly)
	EUDPointType Type;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionId;
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ValueParameters = {};
	UPROPERTY(BlueprintReadOnly)
	FString TextParameter;
};

USTRUCT(BlueprintType)
struct FUDDealActionInfo
{
	GENERATED_BODY()
public:
	FUDDealActionInfo() {}
	FUDDealActionInfo(int32 dealUniqueId, int32 actionIndex, FUDDiscussionAction actionBody)
		: DealUniqueId(dealUniqueId), ActionIndex(actionIndex), ActionBody(actionBody) {}
	UPROPERTY(BlueprintReadOnly)
	int32 DealUniqueId;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionIndex;
	UPROPERTY(BlueprintReadOnly)
	FUDDiscussionAction ActionBody;
};


USTRUCT(BlueprintType)
struct FUDThroneInfo
{
	GENERATED_BODY()
public:
	FUDThroneInfo() {}
	FUDThroneInfo(int32 id) : UsurperId(id) { }
	UPROPERTY(BlueprintReadOnly)
	int32 UsurperId = 0;
	UPROPERTY(BlueprintReadOnly)
	FText UsurperName = FText::GetEmpty();
};


USTRUCT(BlueprintType)
struct FUDGameStateInfo
{
	GENERATED_BODY()
public:
	FUDGameStateInfo() {}
	FUDGameStateInfo(bool isGameOver, int32 winnerId) : IsGameFinished(isGameOver), WinnerId(winnerId) {}
	UPROPERTY(BlueprintReadOnly)
	bool IsGameFinished = false;
	UPROPERTY(BlueprintReadOnly)
	int32 WinnerId = 0;
};

// GetAvailableActions(NONE|TILE|PLAYER)
// FUDAtionTemplate
// - ActionId
// - ActionParams [TAG INVOKER | TAG TARGET | TAG MODIFIER]
// - RANGE
// - INVALID VALUES
// FUDAtionData(ActionId, Invoker, Target) -> SEND

// Layer that cares about UI/AI to Data Conversion
// Does not really care about Actions as a whole thingy
// But still depends on their restrictions
// In theory this serves as another layer of restrictions over actions
// So it's overseeing actions. Action can allow to send gold and be in negative
// But Administrator will say no to this if we want, so it makes much harsher dependencies
// Actions might allow giving permission modifier to enemy
// Action administrator might not allow it as it would not be cool
#define LOCTEXT_NAMESPACE "ActionAdministrator"

/**
 * Invoked everytime an action was decided by this controller.
 */
DECLARE_DELEGATE_OneParam(UserActionRequestedDelegate, FUDActionData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDOnDataChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUDOnDataReloaded);

/**
 * Overseeing state with many functions for conversion to Info.
 * Allows Player to get easily readable information and confirmations.
 */
UCLASS(Blueprintable)
class UNREALDIPLOMACY_API UUDActionAdministrator : public UObject
{
	GENERATED_BODY()
private:
	FUDNationInfo FUDNationInfo_INVALID = FUDNationInfo(-1, -1);
public:
	/**
	 * Delegate that is binded by player controller.
	 * TODO bind also AI controller and merge it with the delegate that is AI using a.t.m.?
	 */
	UserActionRequestedDelegate OnUserActionRequestedDelegate;
	/**
	 * Allows models to subscribe their update function to automatically update.
	 */
	FUDOnDataChanged OnDataChangedEvent;
	/**
	 * Allows models to subscribe their update function to automatically update.
	 */
	FUDOnDataReloaded OnDataReloadedEvent;
	/**
	 * Notifies all viewmodel subscribers about the subtle change.
	 */
	UFUNCTION()
	void OnDataChanged(const FUDActionData& action)
	{
		OnDataChangedEvent.Broadcast();
	}
	/**
	 * Notifies all viewmodel subscribers about the reload.
	 */
	UFUNCTION()
	void OnDataReloaded()
	{
		OnDataReloadedEvent.Broadcast();
	}
	/**
	 * Request
	 * TODO refactor all these delegates
	 */
	void RequestAction(FUDActionData data)
	{
		OnUserActionRequestedDelegate.ExecuteIfBound(data);
	}
	/**
	 * Defines state that is being monitored and used for determining validity, actions, infos & etc.
	 */
	void SetOverseeingState(TObjectPtr<UUDWorldState> overseeingState)
	{
		if (IsOverseeingStatePresent())
		{
			UE_LOG(LogTemp, Log, 
				TEXT("UUDActionAdministrator(%d): Multiple attempts to set overseeing state."), 
				OverseeingState->FactionPerspective);
			return;
		}
		OverseeingState = overseeingState;
	}
	/**
	 * Alternatively call to IsGameInProgress, that's useable for game that is not yet over.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsGameFinished()
	{
		if (IsOverseeingStatePresent() && 
			OverseeingState->WorldSimulationState == EUDWorldSimulationState::Finished)
		{
			return true;
		}
		return false;
	}
	/**
	 * Validate if the game is in progress.
	 * Should be called before determining anything else about the world.
	 * Alternatively call IsGameFinished();
	 */
	UFUNCTION(BlueprintCallable)
	bool IsGameInProgress()
	{
		if (IsOverseeingStatePresent() && 
			OverseeingState->WorldSimulationState == EUDWorldSimulationState::Simulating)
		{
			return true;
		}
		return false;
	}
	/**
	 * Validates if the state has map generated and ready to use.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsMapStateReady()
	{
		return IsValid(OverseeingState->Map) && OverseeingState->Map->Tiles.Num() > 0;
	}
	/**
	 * Validate if the owning player can end turn.
	 */
	UFUNCTION(BlueprintCallable)
	bool CanEndTurn()
	{
		if (OverseeingState->TurnData.PlayingFaction == OverseeingState->FactionPerspective)
		{
			return true;
		}
		return false;
	}
	/**
	 * Returns TurnInfo.
	 */
	UFUNCTION(BlueprintCallable)
	FUDTurnInfo GetCurrentTurnState()
	{
		return FUDTurnInfo(OverseeingState->TurnData.Turn, OverseeingState->TurnData.PlayingFaction);
	}
	/**
	 * Checks if supplied Id belongs to valid nation.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsValidNation(int32 nationId)
	{
		return OverseeingState->Factions.Contains(nationId);
	}
	/**
	 * Returns NationInfo.
	 */
	UFUNCTION(BlueprintCallable)
	FUDNationInfo GetCurrentNationState(int32 nationId)
	{
		if (IsValidNation(nationId))
			return FUDNationInfo(nationId, OverseeingState->Factions[nationId]->ResourceGold);
		return FUDNationInfo_INVALID;
	}
	/**
	 * Retrieves current gold resource ? TODO this should be converted to generic call for any resource (if we have more then one).
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentResourceGold()
	{
		return OverseeingState->Factions[OverseeingState->FactionPerspective]->ResourceGold;
	}
	/**
	 * Returns list of players in PlayerInfo format.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetPlayerList()
	{
		TArray<FUDPlayerInfo> infos;

		for (auto state : OverseeingState->Factions)
		{
			FUDPlayerInfo player = FUDPlayerInfo(state.Value->PlayerUniqueId);
			player.Name = FText::Format(LOCTEXT("Player", "Player {0}"), state.Value->PlayerUniqueId);
			infos.Add(player);
		}

		return infos;
	}
	/**
	 * Returns TileInfo.
	 */
	UFUNCTION(BlueprintCallable)
	FUDTileInfo GetCurrentTileState(FIntPoint position)
	{
		auto tile = OverseeingState->Map->GetTile(position);
		return FUDTileInfo(tile->OwnerUniqueId, position, tile->Type);
	}
	/**
	 * Returns true if it's owned by world.
	 */
	UFUNCTION(BlueprintCallable)
	bool CanTakeTile(FIntPoint position)
	{
		auto tile = OverseeingState->Map->GetTile(position);
		return tile->OwnerUniqueId == UUDGlobalData::GaiaFactionId;
	}
	/**
	 * Returns true if amount is at least 1 and remains is equal or larger then 0.
	 */
	UFUNCTION(BlueprintCallable)
	bool CanGiveGold(int32 targetId, int32 goldAmount)
	{
		return targetId != OverseeingState->FactionPerspective 
			&& goldAmount > 0 
			&& OverseeingState->Factions[OverseeingState->FactionPerspective]->ResourceGold - goldAmount >= 0;
	}
	UFUNCTION(BlueprintCallable)
	FIntPoint GetFirstNeutralTile()
	{
		for (auto tile : OverseeingState->Map->Tiles)
		{
			if (tile->OwnerUniqueId == UUDGlobalData::GaiaFactionId)
			{
				return tile->Position;
			}
		}
		return FIntPoint(-1, -1);
	}

	/**
	 * Return list of all currently pending requests that require confirmation...
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDActionData> GetPendingRequests()
	{
		TArray<FUDActionData> copy;
		for (auto& itm : OverseeingState->Factions[OverseeingState->FactionPerspective]->PendingRequests)
		{
			copy.Add(itm.Value);
		}
		return copy;
	}
	/**
	 * Return list of all currently pending requests that require confirmation...
	 */
	UFUNCTION(BlueprintCallable)
	int32 GetPendingRequestsCount()
	{
		return OverseeingState->Factions[OverseeingState->FactionPerspective]->PendingRequests.Num();
	}
	UFUNCTION(BlueprintCallable)
	bool IsParticipatingInDeal(int32 dealUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
		{
			return false;
		}
		TObjectPtr<UUDDealState> lastState = OverseeingState->Deals[dealUniqueId];
		// verifies if this person is participating in last existing deal.
		return lastState->Participants.Contains(OverseeingState->FactionPerspective);
	}
	/**
	 * Returns list of active participants, blocked participants, available participants.
	 */
	UFUNCTION(BlueprintCallable)
	FUDDealParticipantsInfo GetDealParticipants(int32 dealUniqueId)
	{
		TArray<FUDPlayerInfo> active;
		TArray<FUDPlayerInfo> blocked;
		TArray<FUDPlayerInfo> available;

		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return FUDDealParticipantsInfo(0, active, blocked, available);

		TObjectPtr<UUDDealState> deal = OverseeingState->Deals[dealUniqueId];
		TArray<FUDPlayerInfo> players = GetPlayerList();

		for (FUDPlayerInfo info : players)
		{
			if (deal->Participants.Contains(info.Id))
			{
				active.Add(info);
			}
			else if (deal->BlockedParticipants.Contains(info.Id))
			{
				blocked.Add(info);
			}
			else 
			{
				available.Add(info);
			}
		}
		return FUDDealParticipantsInfo(dealUniqueId, active, blocked, available);
	}
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetDealIds()
	{
		TArray<int32> keys;
		keys.Empty(0);

		if (OverseeingState->Deals.Num() != 0)
		{
			OverseeingState->Deals.GenerateKeyArray(keys);
			keys.Sort();
		}
		return keys;
	}
	UFUNCTION(BlueprintCallable)
	FUDDealInfo GetDealInfoAnyDEBUG()
	{
		if (OverseeingState->Deals.Num() != 0)
		{
			for (auto& deal : OverseeingState->Deals)
			{
				return FUDDealInfo(deal.Key, deal.Value->DealSimulationState, deal.Value->DealSimulationResult);
			}
		}

		return GetDealInfo(0);
	}
	UFUNCTION(BlueprintCallable)
	bool IsModerator(int32 dealUniqueId)
	{
		return OverseeingState->Deals[dealUniqueId]->OwnerUniqueId == OverseeingState->FactionPerspective;

	}
	/**
	 * Returns list of active participants, blocked participants, available participants.
	 */
	UFUNCTION(BlueprintCallable)
	FUDDealInfo GetDealInfo(int32 dealUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return FUDDealInfo(0, EUDDealSimulationState::Undefined, EUDDealSimulationResult::Undefined);

		TObjectPtr<UUDDealState> deal = OverseeingState->Deals[dealUniqueId];
		return FUDDealInfo(deal->UniqueDealId, deal->DealSimulationState, deal->DealSimulationResult);
	}
	UFUNCTION(BlueprintCallable)
	bool IsParticipantInCurrentDeal(int32 dealUniqueId, int32 playerId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return false;
		if (OverseeingState->Deals[dealUniqueId]->Participants.Num() == 0)
			return false;
		return OverseeingState->Deals[dealUniqueId]->Participants.Contains(playerId);
	}

	UFUNCTION(BlueprintCallable)
	bool IsReadyInCurrentDeal(int32 dealUniqueId, int32 playerId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return false;
		if (OverseeingState->Deals[dealUniqueId]->IsReadyPlayerList.Num() == 0)
			return false;
		return OverseeingState->Deals[dealUniqueId]->IsReadyPlayerList.Contains(playerId);
	}

	UFUNCTION(BlueprintCallable)
	int32 GetReadyParticipantCount(int32 dealUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return false;
		if (OverseeingState->Deals[dealUniqueId]->IsReadyPlayerList.Num() == 0)
			return false;
		return OverseeingState->Deals[dealUniqueId]->IsReadyPlayerList.Num();
	}

	UFUNCTION(BlueprintCallable)
	bool IsPositiveVotingCurrentDeal(int32 dealUniqueId, int32 playerId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return false;
		if (OverseeingState->Deals[dealUniqueId]->PositiveVotePlayerList.Num() == 0)
			return false;
		return OverseeingState->Deals[dealUniqueId]->PositiveVotePlayerList.Contains(playerId);
	}

	UFUNCTION(BlueprintCallable)
	int32 GetPositiveVoteCurrentDealCount(int32 dealUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return false;
		if (OverseeingState->Deals[dealUniqueId]->PositiveVotePlayerList.Num() == 0)
			return false;
		return OverseeingState->Deals[dealUniqueId]->PositiveVotePlayerList.Num();
	}

	UFUNCTION(BlueprintCallable)
	int32 GetParticipantCount(int32 dealUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return false;
		if (OverseeingState->Deals[dealUniqueId]->Participants.Num() == 0)
			return false;
		return OverseeingState->Deals[dealUniqueId]->Participants.Num();
	}

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetDealMessages(int32 dealUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 || !OverseeingState->Deals.Contains(dealUniqueId))
			return TArray<FString>();
		return OverseeingState->Deals[dealUniqueId]->ChatHistory;
	}

	// structs do not support recursion so we need to make it little less recursive...
	UFUNCTION(BlueprintCallable)
	TArray<FUDDealPointChildInfo> GetDealPointChildTree(int32 dealUniqueId, int32 pointUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 ||
			!OverseeingState->Deals.Contains(dealUniqueId) ||
			OverseeingState->Deals[dealUniqueId]->Points.Num() == 0 ||
			!OverseeingState->Deals[dealUniqueId]->Points.Contains(pointUniqueId))
			return TArray<FUDDealPointChildInfo>();

		TArray<FUDDealPointChildInfo> childs;

		for (auto consequencePointId : OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->Consequencies)
		{
			childs.Add(FUDDealPointChildInfo(dealUniqueId, consequencePointId));
		}

		return childs;
	}

	UFUNCTION(BlueprintCallable)
	FUDDealPointTreeInfo GetDealPointsTree(int32 dealUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 ||
			!OverseeingState->Deals.Contains(dealUniqueId))
			return FUDDealPointTreeInfo(0, 0, {});

		TArray<FUDDealPointChildInfo> childs;

		for (auto primaryPointId : OverseeingState->Deals[dealUniqueId]->PrimaryPoints)
		{
			childs.Add(FUDDealPointChildInfo(dealUniqueId, primaryPointId));
		}
		int32 totalCount = OverseeingState->Deals[dealUniqueId]->Points.Num();
		return FUDDealPointTreeInfo(dealUniqueId, totalCount, childs);
	}
	UFUNCTION(BlueprintCallable)
	FUDDealPointInfo GetDealPointInfo(int32 dealUniqueId, int32 pointUniqueId)
	{
		if (OverseeingState->Deals.Num() == 0 ||
			!OverseeingState->Deals.Contains(dealUniqueId) ||
			!OverseeingState->Deals[dealUniqueId]->Points.Contains(pointUniqueId))
			return FUDDealPointInfo(0, 0, EUDPointType::Error, 0);
		return FUDDealPointInfo(dealUniqueId, pointUniqueId, 
			OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->Type,
			OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->ActionId,
			OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->ValueParameters,
			OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->TextParameter);
	}

	/**
	 * Returns list of players in PlayerInfo format.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetInvokerPointPlayerList(int32 dealUniqueId, int32 pointUniqueId)
	{
		TArray<FUDPlayerInfo> infos;

		for (auto playerId : OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->Invokers)
		{
			auto state = OverseeingState->Factions[playerId];

			FUDPlayerInfo player = FUDPlayerInfo(state->PlayerUniqueId);
			player.Name = FText::Format(LOCTEXT("Player", "Player {0}"), state->PlayerUniqueId);
			infos.Add(player);
		}

		return infos;
	}
	/**
	 * Returns list of players in PlayerInfo format.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetTargetPointPlayerList(int32 dealUniqueId, int32 pointUniqueId)
	{
		TArray<FUDPlayerInfo> infos;

		for (auto playerId : OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->Targets)
		{
			auto state = OverseeingState->Factions[playerId];

			FUDPlayerInfo player = FUDPlayerInfo(state->PlayerUniqueId);
			player.Name = FText::Format(LOCTEXT("Player", "Player {0}"), state->PlayerUniqueId);
			infos.Add(player);
		}

		return infos;
	}

	/**
	 * Returns list of actions that will be created from this deal, without any filter.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDActionData> GetDealPointsAsUnfoldedActions(int32 dealUniqueId)
	{
		return UUDDealActionContractCreate::FinalizeActions(OverseeingState, dealUniqueId);
	}

	UFUNCTION(BlueprintCallable)
	TArray<FUDDealActionInfo> GetAllActionForCurrentPlayer(int32 dealUniqueId)
	{
		TArray<FUDDealActionInfo> actions;
		
		for (size_t index = 0; index < OverseeingState->Deals[dealUniqueId]->DealActionList.Num(); index++)
		{
			FUDDiscussionAction action = OverseeingState->Deals[dealUniqueId]->DealActionList[index];
			if (action.Action.InvokerFactionId == OverseeingState->FactionPerspective) {
				actions.Add(FUDDealActionInfo(dealUniqueId, index, action));
			}
		}

		return actions;
	}

	UFUNCTION(BlueprintCallable)
	TArray<FUDDealActionInfo> GetAllActionForOtherPlayers(int32 dealUniqueId)
	{
		TArray<FUDDealActionInfo> actions;

		for (size_t index = 0; index < OverseeingState->Deals[dealUniqueId]->DealActionList.Num(); index++)
		{
			FUDDiscussionAction action = OverseeingState->Deals[dealUniqueId]->DealActionList[index];
			if (action.Action.InvokerFactionId != OverseeingState->FactionPerspective) {
				actions.Add(FUDDealActionInfo(dealUniqueId, index, action));
			}
		}

		return actions;
	}

	UFUNCTION(BlueprintCallable)
	bool IsDealPointInvoker(int32 dealUniqueId, int32 pointUniqueId, int32 playerId)
	{
		return OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->Invokers.Contains(playerId);
	}
	UFUNCTION(BlueprintCallable)
	bool IsDealPointTarget(int32 dealUniqueId, int32 pointUniqueId, int32 playerId)
	{
		return OverseeingState->Deals[dealUniqueId]->Points[pointUniqueId]->Targets.Contains(playerId);
	}

	/**
	 * Returns ThroneInfo that holds id and name of current player on throne..
	 */
	UFUNCTION(BlueprintCallable)
	FUDThroneInfo GetThroneInfo()
	{
		FUDThroneInfo info = FUDThroneInfo(OverseeingState->ImperialThrone.Ruler);
		
		if (info.UsurperId == UUDGlobalData::GaiaFactionId)
		{
			info.UsurperName = FText::Format(LOCTEXT("Player", "Throne is empty. ({0})"), info.UsurperId);
		}
		else
		{
			// TODO transform this to full player name...
			info.UsurperName = FText::Format(LOCTEXT("Player", "Player {0}"), info.UsurperId);
		}
		return info;
	}

	UFUNCTION(BlueprintCallable)
	FUDGameStateInfo GetGameStateInfo()
	{
		return FUDGameStateInfo(
			OverseeingState->WorldSimulationState == EUDWorldSimulationState::Finished,
			OverseeingState->ImperialThrone.Ruler
		);
	}


	UFUNCTION(BlueprintCallable)
	bool IsLocalPlayerWinner()
	{
		return 
			OverseeingState->WorldSimulationState == EUDWorldSimulationState::Finished &&
			OverseeingState->ImperialThrone.Ruler == OverseeingState->FactionPerspective;
	}


	UFUNCTION(BlueprintCallable)
	bool CanUsurpThrone()
	{
		return OverseeingState->ImperialThrone.Ruler == UUDGlobalData::GaiaFactionId;
	}

	UFUNCTION(BlueprintCallable)
	bool CanAbdicateThrone()
	{
		return OverseeingState->ImperialThrone.Ruler == OverseeingState->FactionPerspective;
	}

public:
	TObjectPtr<UUDMapState> GetMapState();
	/**
	 * Returns new action that can be invoked.
	 */
	//UFUNCTION(BlueprintCallable)
	FUDActionData GetAction(int32 actionId);
	/**
	 * Returns new action that can be invoked.
	 */
	//UFUNCTION(BlueprintCallable)
	FUDActionData GetAction(int32 actionId, TArray<int32> optionalValues);
	/**
	 * Returns new action that can be invoked.
	 */
	//UFUNCTION(BlueprintCallable)
	FUDActionData GetAction(int32 actionId, TArray<int32> optionalValues, FString optionaString);
	/**
	 * Returns new action that can be invoked.
	 */
	//UFUNCTION(BlueprintCallable)
	FUDActionData GetAction(int32 actionId, FString optionaString);
	/**
	 * Returns new action that can be invoked.
	 */
	//UFUNCTION(BlueprintCallable)
	FUDActionData GetAcceptAction(int32 actionId, FUDActionData sourceAction);
	/**
	 * Returns new action that can be invoked.
	 */
	 //UFUNCTION(BlueprintCallable)
	FUDActionData GetRejectAction(int32 actionId, FUDActionData sourceAction);
protected:
	UFUNCTION()
	bool IsOverseeingStatePresent() { return IsValid(OverseeingState); }
	UPROPERTY()
	TObjectPtr<UUDWorldState> OverseeingState;
	//UPROPERTY()
	//TObjectPtr<UUDActionManager> ActionManager;
};
#undef LOCTEXT_NAMESPACE