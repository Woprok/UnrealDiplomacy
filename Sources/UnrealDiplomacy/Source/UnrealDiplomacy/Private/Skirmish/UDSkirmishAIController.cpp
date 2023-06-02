// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishAIController.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"
#include "Core/Simulation/Actions/UDGameActionTileTake.h"
#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"

AUDSkirmishAIController::AUDSkirmishAIController()
{
	Type = EUDControllerType::AI;
}

void AUDSkirmishAIController::ProcessOutTurnPlay()
{
	// Resolve requests
	ResolveRequests();
}

void AUDSkirmishAIController::ProcessInTurnPlay()
{
	if (GetAdministrator()->IsMapStateReady())
	{
		FIntPoint tile = GetAdministrator()->GetFirstNeutralTile();
		if (tile != FIntPoint(-1, -1))
		{
			OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDGameActionTileTake::ActionTypeId,
				{
					UUDGlobalData::GaiaId, tile.X, tile.Y
				}));
		}
	}

	// Naively give first player gold, TODO remove this or replace with some logic
	if (GetAdministrator()->GetCurrentResourceGold() > 100 && gifters.Num() > 0)
	{
		OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDGameActionGift::ActionTypeId,
			{ 
				gifters.Pop(), 
				69 
			}
		));
	}

	// Resolve requests
	ResolveRequests();

	// AI can take throne, that's completely natural thing to do if you are rich!
	if (GetAdministrator()->GetCurrentResourceGold() >= 500 && GetAdministrator()->CanUsurpThrone())
	{
		OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDGameActionThroneUsurp::ActionTypeId));
	}

	// Finish this by executing end turn action, thus giving up control.
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(UUDSystemActionTurnEnd::ActionTypeId));
}

void AUDSkirmishAIController::ResolveRequests()
{
	for (auto request : GetAdministrator()->GetPendingRequests())
	{
		// Handle Gifts
		if (request.ActionTypeId == UUDGameActionGift::ActionTypeId)
		{
			gifters.Add(request.InvokerFactionId);
			OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAcceptAction(UUDGameActionGiftAccept::ActionTypeId,
				request));
		}
	}
}