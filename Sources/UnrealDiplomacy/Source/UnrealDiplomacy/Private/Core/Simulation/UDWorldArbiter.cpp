// Copyright Miroslav Valach

#include "Core/Simulation/UDWorldArbiter.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "Core/Simulation/Actions/UDGameActionThroneReceive.h"

bool UUDWorldArbiter::OnActionExecutionFinished(int32 actionType, TObjectPtr<UUDWorldState> gaiaWorldState)
{
	if (GameReachedEnd)
	{
		// Execution must be halted if game was decided.
		return false;
	}

	switch (actionType)
	{
	case UUDSystemActionTurnEnd::ActionTypeId:
		EvaluateTurnGameOverState(gaiaWorldState);
	default:
		// We don't care about the remaining actions.
		break;
	}

	return GameReachedEnd;
}

TArray<FUDActionData> UUDWorldArbiter::EndGame()
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

void UUDWorldArbiter::EvaluateTurnGameOverState(TObjectPtr<UUDWorldState> gaiaWorldState)
{
	// Winner was found
	if (gaiaWorldState->TurnData.Turn < CurrentRuleSet.MaxTurnCount)
	{
		return;
	}
	// Game will end with this action.
	GameReachedEnd = true;
	// New ruler will be crowned.
	if (gaiaWorldState->ImperialThrone.Ruler == UUDGlobalData::GaiaFactionId)
	{
		CrownIsEmpty = true;
		// TODO add legitimacy
		// This determines final usurper based on gold
		int32 mostGold = -1;
		for (auto& player : gaiaWorldState->Factions)
		{
			if (player.Value->ResourceGold > mostGold && player.Value->PlayerUniqueId != UUDGlobalData::GaiaFactionId)
			{
				mostGold = player.Value->ResourceGold;
				CrownableRuler = player.Value->PlayerUniqueId;
			}
		}
	}
}

FUDActionData UUDWorldArbiter::DetermineNewRuler()
{
	return FUDActionData(UUDGameActionThroneReceive::ActionTypeId, UUDGlobalData::GaiaFactionId, { CrownableRuler });
}

FUDActionData UUDWorldArbiter::CreateEndGame()
{
	return FUDActionData(UUDSystemActionTurnEnd::ActionTypeId, UUDGlobalData::GaiaFactionId);
}