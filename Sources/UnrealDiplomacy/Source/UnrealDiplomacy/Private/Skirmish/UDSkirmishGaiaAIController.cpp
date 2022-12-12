// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Skirmish/UDSkirmishGameMode.h"

void AUDSkirmishGaiaAIController::OnActionExecuted(FUDActionData& executedAction)
{
	// Simulator called this after action was executed
	// We have to pass our action if its our turn
	// We are quite lazy so let's do the simplest one for now.

	// After start of the game do some exclusive actions.
	if (executedAction.ActionTypeId == UUDStartGameAction::ActionTypeId)
	{
		FUDActionData createResources(UUDGenerateIncomeAction::ActionTypeId, GetControllerUniqueId());
		OnActionDecidedDelegate.ExecuteIfBound(createResources);
		return;
	}

	// End turn once income was generated for the first time.
	if (executedAction.ActionTypeId == UUDGenerateIncomeAction::ActionTypeId)
	{
		FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
		OnActionDecidedDelegate.ExecuteIfBound(endTurn);
		return;
	}
	
	// End turn automatically as there is no other action to await for.
	if (executedAction.ActionTypeId == UUDEndTurnAction::ActionTypeId && GaiaState->CurrentTurnPlayerId == GetControllerUniqueId())
	{
		FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
		//OnActionDecidedDelegate.ExecuteIfBound(endTurn);
		// TODO THIS REQUIRES AT LEAST ONE PLAYER OR AI WILL NEVER END 
		return;
	}
}