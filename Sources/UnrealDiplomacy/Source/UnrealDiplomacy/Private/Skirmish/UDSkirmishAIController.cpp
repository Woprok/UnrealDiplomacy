// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishAIController.h"
#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"
#include "Core/Simulation/Actions/UDGameActionTileTake.h"
#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"

void AUDSkirmishAIController::InitializeAdministrator()
{
	InternalPersonalAdministrator = NewObject<UUDActionAdministrator>();
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGaiaAIController(%d): Initialized personal administrator."), GetControllerUniqueId());
}

void AUDSkirmishAIController::OnActionExecuted(FUDActionData executedAction)
{
	// TODO PREPROCESSING FOR executedAction

	// AI is not allowed to act before the game is in progress.
	// AI is not allowed to act after the game is finished.
	if (!GetPersonalAdministrator()->IsGameInProgress())
	{
		return;
	}

	// AI act's only on their respective turn.
	if (!GetPersonalAdministrator()->CanEndTurn())
	{
		return;
	}
	// AI can make new decisions only once on their respective turn.
	if (IsPlaying)
	{
		// Prevents repeated nesting as it forces AI code to break.
		return;
	}
	IsPlaying = true;
	DoTurn();
	IsPlaying = false;
	return;
}

void AUDSkirmishAIController::DoTurn()
{
	if (GetPersonalAdministrator()->IsMapStateReady())
	{
		FIntPoint tile = GetPersonalAdministrator()->GetFirstNeutralTile();
		if (tile != FIntPoint(-1, -1))
		{
			OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetAction(UUDGameActionTileTake::ActionTypeId, 
				{
					UUDGlobalData::GaiaId, tile.X, tile.Y
				}));
		}
	}

	// Naively give first player gold, TODO remove this or replace with some logic
	if (GetPersonalAdministrator()->GetCurrentResourceGold() > 100 && gifters.Num() > 0)
	{
		OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetAction(UUDGameActionGift::ActionTypeId,
			{ 
				gifters.Pop(), 
				69 
			}
		));
	}

	// Resolve requests
	for (auto request : GetPersonalAdministrator()->GetPendingRequests())
	{
		if (request.ActionTypeId == UUDGameActionGift::ActionTypeId)
		{
			gifters.Add(request.InvokerFactionId);
			OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetAcceptAction(UUDGameActionGiftAccept::ActionTypeId, 
				request));
		}
	}

	// AI can take throne, that's completely natural thing to do!
	if (GetPersonalAdministrator()->GetCurrentResourceGold() >= 500 && GetPersonalAdministrator()->CanUsurpThrone())
	{
		OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetAction(UUDGameActionThroneUsurp::ActionTypeId));
	}

	// Finish this by executing end turn action, thus giving up control.
	OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetAction(UUDSystemActionTurnEnd::ActionTypeId));

	// This should contain decisions, or alternatively if we reach a point where behaviour tree is used
	// this should be left empty :)
	// TODO behaviour tree

}