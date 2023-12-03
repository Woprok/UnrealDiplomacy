// Copyright Miroslav Valach
// TODO consider changing OnActionExecuted to async.
// TODO consider changing it to behaviour tree.

#include "Core/UDAIController.h"
#include "Core/UDGameInstance.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDActionAdministrator.h"

void AUDAIController::SetControllerUniqueId(int32 uniqueControllerId)
{
	UniqueControllerId = uniqueControllerId;
}

int32 AUDAIController::GetControllerUniqueId()
{
	return UniqueControllerId;
}

void AUDAIController::SetControlledFactionId(int32 factionId)
{
	ControlledFactionId = factionId;
}

int32 AUDAIController::GetControlledFactionId()
{
	return ControlledFactionId;
}

void AUDAIController::OnActionExecuted(FUDActionData executedAction)
{
	if (!GetAdministrator()->IsOverseeingStatePresent())
	{
		return;
	}

	// AI is never allowed to act after the game is finished.
	if (GetAdministrator()->IsGameOver())
	{
		return;
	}

	// AI is allowed to access actions outside of the turn during the game and pregame.
	ProcessPlayableAction(executedAction);

	// AI is supposed to act primarily during the game.
	// This allows AI to act before the game is in progress.
	// For example creating custom stratagem strategy.
	if (!GetAdministrator()->IsGamePlayed())
	{
		if (!PreGamePlay)
		{
			PreGameExecution();
		}
		return;
	}


	// AI is always halted if game is in intermezzo.
	// This allows AI to do something during intermezzo, but it should never try to invoke new actions.
	if (GetAdministrator()->IsIntermezzo())
	{
		if (!IntermezzoPlay)
		{
			IntermezzoExecution();
		}
		return;
	}
	
	// AI acts primarily at the start of the turn.
	if (GetAdministrator()->CanFinishTurn())
	{
		// AI can start making new decisions only once per turn.
		// Prevents repeated nesting as it forces AI code to break.
		if (!InTurnPlay)
		{
			InTurnExecution();
		}
		return;
	}
	// AI is allowed to act after finishing the turn, but it should be used with caution.
	else
	{
		// AI can make new decisions in response to other players play after it finished turn.
		// Prevents repeated nesting as it forces AI code to break.
		if (!OutTurnPlay)
		{
			OutTurnExecution();
		}
		return;
	}
}

void AUDAIController::SetSimulatedStateAccess(TObjectPtr<UUDWorldState> factionWorldState)
{
	UE_LOG(LogTemp, Log, TEXT("AUDAIController(%d): Obtained state for faction(%d) as controller of faction(%d)."), 
		GetControllerUniqueId(),
		factionWorldState->FactionPerspective,
		GetControlledFactionId());
	FactionState = factionWorldState;
	GetAdministrator()->SetOverseeingState(factionWorldState);
}

void AUDAIController::InitializeAdministrator()
{
	ActionAdministrator = NewObject<UUDActionAdministrator>();
	ActionAdministrator->SetActionManager(UUDGameInstance::Get(GetWorld())->GetActionManager());
	UE_LOG(LogTemp, Log, TEXT("AUDAIController(%d): Initialized administrator."), GetControllerUniqueId());
}

TObjectPtr<UUDActionAdministrator> AUDAIController::GetAdministrator()
{
	if (!IsValid(ActionAdministrator))
	{
		InitializeAdministrator();
	}
	return ActionAdministrator;
}

void AUDAIController::ProcessPlayableAction(const FUDActionData& executedAction)
{
	return;
}

void AUDAIController::ProcessPreGamePlay()
{
	return;
}

void AUDAIController::ProcessInTurnPlay()
{
	return;
}

void AUDAIController::ProcessOutTurnPlay()
{
	return;
}

void AUDAIController::ProcessIntermezzoPlay()
{
	return;
}

void AUDAIController::PreGameExecution()
{
	PreGamePlay = true;
	ProcessPreGamePlay();
	PreGamePlay = false;
}

void AUDAIController::InTurnExecution()
{
	InTurnPlay = true;
	ProcessInTurnPlay();
	InTurnPlay = false;
}

void AUDAIController::OutTurnExecution()
{
	OutTurnPlay = true;
	ProcessOutTurnPlay();
	OutTurnPlay = false;
}

void AUDAIController::IntermezzoExecution()
{
	IntermezzoPlay = true;
	ProcessIntermezzoPlay();
	IntermezzoPlay = false;
}

void AUDAIController::MakeAction(int32 actionId)
{
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(actionId));
}

void AUDAIController::MakeAction(int32 actionId, TArray<int32> values)
{
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetAction(actionId, values));
}

void AUDAIController::MakeConfirmAction(int32 decisionId)
{
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetConfirmAction(decisionId));
}

void AUDAIController::MakeDeclineAction(int32 decisionId)
{
	OnActionDecidedDelegate.ExecuteIfBound(GetAdministrator()->GetDeclineAction(decisionId));
}