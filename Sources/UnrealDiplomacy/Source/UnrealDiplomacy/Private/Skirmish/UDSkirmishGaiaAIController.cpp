// Copyright Miroslav Valach


#include "Skirmish/UDSkirmishGaiaAIController.h"
#include "Skirmish/UDSkirmishGameMode.h"

void AUDSkirmishGaiaAIController::InitializeAdministrator()
{
	InternalPersonalAdministrator = NewObject<UUDActionAdministrator>();
	UE_LOG(LogTemp, Log, TEXT("AUDSkirmishGaiaAIController(%d): Initialized personal administrator."), GetControllerUniqueId());
}

void AUDSkirmishGaiaAIController::OnActionExecuted(FUDActionData executedAction)
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

void AUDSkirmishGaiaAIController::DoTurn()
{
	// Finish this by executing end turn action, thus giving up control.
	OnActionDecidedDelegate.ExecuteIfBound(GetPersonalAdministrator()->GetEndTurnAction());

	// Evaluation always ends

	// Simulator called this after action was executed
	// We have to pass our action if its our turn
	// We are quite lazy so let's do the simplest one for now.

	// After start of the game do some exclusive actions.
	//if (executedAction.ActionTypeId == UUDStartGameAction::ActionTypeId)
	//{
	//	FUDActionData createResources(UUDGenerateIncomeAction::ActionTypeId, GetControllerUniqueId());
	//	OnActionDecidedDelegate.ExecuteIfBound(createResources);
	//	return;
	//}
	//
	//// End turn once income was generated for the first time.
	//if (executedAction.ActionTypeId == UUDGenerateIncomeAction::ActionTypeId)
	//{
	//	FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
	//	OnActionDecidedDelegate.ExecuteIfBound(endTurn);
	//	return;
	//}
	//
	//// End turn automatically as there is no other action to await for.
	//if (executedAction.ActionTypeId == UUDEndTurnAction::ActionTypeId && GaiaState->CurrentTurnPlayerId == GetControllerUniqueId())
	//{
	//	FUDActionData endTurn(UUDEndTurnAction::ActionTypeId, GetControllerUniqueId());
	//	//OnActionDecidedDelegate.ExecuteIfBound(endTurn);
	//	// TODO THIS REQUIRES AT LEAST ONE PLAYER OR AI WILL NEVER END 
	//	return;
	//}
}