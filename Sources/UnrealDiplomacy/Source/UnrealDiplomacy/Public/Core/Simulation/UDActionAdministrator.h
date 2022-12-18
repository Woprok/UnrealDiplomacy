// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionInterface.h"
#include "UDWorldState.h"
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
	FUDPlayerInfo(int32 Id) : Id(Id) { }
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FText Name = FText::GetEmpty();
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
 * Overseeing state with many functions for conversion to Info.
 * Allows Player to get easily readable information and confirmations.
 */
UCLASS(Blueprintable)
class UNREALDIPLOMACY_API UUDActionAdministrator : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Defines state that is being monitored and used for determining validity, actions, infos & etc.
	 */
	void SetOverseeingState(TObjectPtr<UUDWorldState> overseeingState)
	{
		if (IsOverseeingStatePresent())
		{
			UE_LOG(LogTemp, Log, 
				TEXT("UUDActionAdministrator(%d): Multiple attempts to set overseeing state."), 
				OverseeingState->PerspectivePlayerId);
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
			OverseeingState->WorldSimulationState == EUDWorldSimulationState::FINISHING)
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
			OverseeingState->WorldSimulationState == EUDWorldSimulationState::PLAYING)
		{
			return true;
		}
		return false;
	}
	/**
	 * Validate if the owning player can end turn.
	 */
	UFUNCTION(BlueprintCallable)
	bool CanEndTurn()
	{
		if (OverseeingState->CurrentTurnPlayerId == OverseeingState->PerspectivePlayerId)
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
		return FUDTurnInfo(OverseeingState->CurrentTurn, OverseeingState->CurrentTurnPlayerId);
	}
	/**
	 * Retrieves end turn action, that is used for execution in simulation.
	 */
	UFUNCTION(BlueprintCallable)
	FUDActionData GetEndTurnAction()
	{
		return FUDActionData(UUDEndTurnAction::ActionTypeId, OverseeingState->PerspectivePlayerId);
	}
	/**
	 * Returns list of players in PlayerInfo format.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<FUDPlayerInfo> GetPlayerList()
	{
		TArray<FUDPlayerInfo> infos;

		for (auto state : OverseeingState->Players)
		{
			FUDPlayerInfo player = FUDPlayerInfo(state.Value->PlayerUniqueId);
			player.Name = FText::Format(LOCTEXT("Player", "Player {0}"), state.Value->PlayerUniqueId);
			infos.Add(player);
		}

		return infos;
	}
protected:
	UFUNCTION()
	bool IsOverseeingStatePresent() { return IsValid(OverseeingState); }
	UPROPERTY()
	TObjectPtr<UUDWorldState> OverseeingState;
};
#undef LOCTEXT_NAMESPACE