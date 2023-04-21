// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.h"
#include "UDActionInterface.h"
#include "UDWorldState.h"
#include "UDWorldArbiter.generated.h"

USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDArbiterRuleset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	int32 MaxTurnCount = 12;
};

/**
 * The only absolute authority over the execution flow.
 * Is responsible for determining if the WorldState reached a point of victory.
 * There is currently only one way to win the game by being the holder of the throne.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDWorldArbiter : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Requires Gaia world state to provide accurate information.
	 */
	bool OnActionExecutionFinished(int32 actionType, TObjectPtr<UUDWorldState> gaiaWorldState)
	{
		if (GameReachedEnd)
		{
			// Execution must be halted if game was decided.
			return false;
		}

		switch (actionType)
		{
		case UUDEndTurnAction::ActionTypeId:
			EvaluateTurnGameOverState(gaiaWorldState);
		default:
			// We don't care about the remaining actions.
			break;
		}

		return GameReachedEnd;
	}

	TArray<FUDActionData> EndGame()
	{
		TArray<FUDActionData> actions;
		// Return if game is still not finished based on last check.
		if (!GameReachedEnd)
			return actions;

		if (CrownIsEmpty)
		{
			actions.Add(DetermineNewRuler());
		}

		actions.Add(CreateEndGame());
		// This is final action in sequence always.
		// Endgame is expected to behave as hard stop for taking new actions.
		return actions;
	}
protected:
	void EvaluateTurnGameOverState(TObjectPtr<UUDWorldState> gaiaWorldState)
	{
		// Winner was found
		if (gaiaWorldState->CurrentTurn < CurrentRuleSet.MaxTurnCount)
		{
			return;
		}
		// Game will end with this action.
		GameReachedEnd = true;
		// New ruler will be crowned.
		if (gaiaWorldState->ImperialThrone.Ruler == UUDWorldState::GaiaWorldStateId)
		{
			CrownIsEmpty = true;
			// TODO add legitimacy
			// This determines final usurper based on gold
			int32 mostGold = -1;
			for (auto& player : gaiaWorldState->Players)
			{
				if (player.Value->ResourceGold > mostGold)
				{
					CrownableRuler = player.Value->PlayerUniqueId;
				}
			}
		}
	}
	FUDActionData DetermineNewRuler()
	{
		return FUDActionData(UUDEndGameAction::ActionTypeId, UUDWorldState::GaiaWorldStateId, { CrownableRuler });
	}
	FUDActionData CreateEndGame()
	{
		return FUDActionData(UUDEndGameAction::ActionTypeId, UUDWorldState::GaiaWorldStateId);
	}
private:
	bool GameReachedEnd = false;
	bool CrownIsEmpty = false;
	int32 CrownableRuler = 0;
	FUDArbiterRuleset CurrentRuleSet = FUDArbiterRuleset();
};