// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishAIController.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDGameActionTileTake.h"
#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/Simulation/Actions/UDSystemActionTurnFinish.h"
#include "Core/Simulation/Actions/UDGameActionThroneSupport.h"

void AUDSkirmishAIController::ProcessOutTurnPlay()
{
	// Resolve requests
	ResolveRequests();
}

// TODO create dedicated AI retrieves from world state without UI junk...
void AUDSkirmishAIController::ProcessInTurnPlay()
{
	// Resolve requests
	ResolveRequests();

	const auto res = GetAdministrator()->GetLocalFactionResourceList();
	const auto throne = GetAdministrator()->GetThroneInfo();
	
	// Try taking the throne or support your favorite if someone claimed it.
	if (throne.State == EUDThroneState::Empty && res[UD_RESOURCE_REPUTATION_ID].Amount >= 500)
	{
		MakeAction(UUDGameActionThroneUsurp::ActionTypeId);
	}
	else if (throne.State == EUDThroneState::Usurper)
	{
		MakeAction(UUDGameActionThroneSupport::ActionTypeId, { FavoriteFaction });
	}

	// Try sharing the wealth...
	if (res[UD_RESOURCE_GOLD_ID].Amount >= 1000)
	{
		TArray<int32> smallGoldGift = { FavoriteFaction,
			UD_RESOURCE_GOLD_ID, res[UD_RESOURCE_GOLD_ID].Amount / 10
		};
		MakeAction(UUDGameActionGift::ActionTypeId, smallGoldGift);
	}

	// Try take some land...
	const auto tiles = GetAdministrator()->GetNeutralTiles();
	if (tiles.Num() > 0)
	{
		MakeAction(UUDGameActionTileTake::ActionTypeId, { tiles[0].Position.X, tiles[0].Position.Y });
	}

	// Finish this by executing finish turn action, thus giving up primary control.
	MakeAction(UUDSystemActionTurnFinish::ActionTypeId);
}

void AUDSkirmishAIController::ResolveRequests()
{
	for (const auto& request : GetAdministrator()->GetAllLocalRequests().Messages)
	{
		switch (request.Content.ActionTypeId)
		{
		// Be happy we have new favorite.
		case UUDGameActionGift::ActionTypeId:
			MakeConfirmAction(request.DecisionId);
			FavoriteFaction = GetAdministrator()->GetPendingRequest(request.DecisionId).InvokerFactionId;
			break;
		// They definitely can't request something bad right ?
		default:
			MakeConfirmAction(request.DecisionId);
			break;
		}
	}
}