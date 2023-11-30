// Copyright Miroslav Valach

#include "Core/Simulation/UDWorldArbiter.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "Core/Simulation/Actions/UDSystemActionTurnFinish.h"
#include "Core/Simulation/Actions/UDSystemActionIntermezzoStart.h"
#include "Core/Simulation/Actions/UDSystemActionIntermezzoEnd.h"
#include "Core/Simulation/Actions/UDSystemActionRegentChange.h"
#include "Core/Simulation/Actions/UDSystemActionGameEnd.h"
#include "Core/Simulation/Actions/UDGameActionThroneReceive.h"
#include "Core/Simulation/Modifiers/UDFactionModifierThroneSupport.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"

bool UUDWorldArbiter::OnActionExecutionFinished(int32 actionType, const TObjectPtr<UUDWorldState>& gaiaWorldState)
{
	if (GameReachedEnd)
	{
		// Execution must be halted if game was decided.
		return false;
	}

	switch (actionType)
	{
	case UUDSystemActionTurnEnd::ActionTypeId:
	case UUDSystemActionIntermezzoStart::ActionTypeId:
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

int32 UUDWorldArbiter::GetTotalSupport(const TObjectPtr<UUDWorldState>& state, int32 factionId)
{
	int32 baseSupport = ResourceManager->GetCurrent(state->Factions[factionId], UUDGameResourceReputation::ResourceId);
	int32 additionalSupport = 0;

	for (const auto& modifier : ModifierManager->GetAllFactionModifiers(
		state->Factions[factionId], 
		UUDFactionModifierThroneSupport::ModifierTypeId))
	{
		additionalSupport += ResourceManager->GetCurrent(state->Factions[modifier.InvokerId], UUDGameResourceReputation::ResourceId);
	}

	return baseSupport + additionalSupport;
}

void UUDWorldArbiter::EvaluateTurnGameOverState(const TObjectPtr<UUDWorldState>& gaiaWorldState)
{
	// Winner was found
	if (gaiaWorldState->TurnData.Turn <= CurrentRuleSet.MaxTurnCount)
	{
		return;
	}
	// Game will end with this action.
	GameReachedEnd = true;
	// New ruler will be crowned.
	if (gaiaWorldState->ImperialThrone.Ruler == UUDGlobalData::GaiaFactionId)
	{
		// Ruler is not present...
		CrownIsEmpty = true;
		// Determine ruler based on reputation order, including any reputation obtained from support.
		// This always selects first player in the order ?
		// Might need to be changed so it has additional conditions to ensure fairness.
		// TODO this should utilize model & manager functions instead of doing raw read.
		int32 currentMaximum = INT32_MIN;
		for (auto& player : gaiaWorldState->Factions)
		{
			int32 currentPlayerSupport = GetTotalSupport(gaiaWorldState, player.Key);

			if (currentPlayerSupport > currentMaximum &&
				player.Value->PlayerUniqueId != UUDGlobalData::GaiaFactionId &&
				player.Value->PlayerUniqueId != UUDGlobalData::ObserverFactionId)
			{
				currentMaximum = currentPlayerSupport;
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
	return FUDActionData(UUDSystemActionGameEnd::ActionTypeId, UUDGlobalData::GaiaFactionId);
}

void UUDWorldArbiter::SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

void UUDWorldArbiter::SetResourceManager(TObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}