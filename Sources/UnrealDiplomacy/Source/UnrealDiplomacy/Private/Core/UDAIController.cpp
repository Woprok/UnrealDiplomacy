// Copyright Miroslav Valach
// TODO consider changing OnActionExecuted to async.
// TODO consider changing it to behaviour tree.

#include "Core/UDAIController.h"
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

void AUDAIController::SetControllerType(EUDControllerType type)
{
	Type = type;
}

EUDControllerType AUDAIController::GetControllerType()
{
	return Type;
}

void AUDAIController::OnActionExecuted(FUDActionData executedAction)
{
	// AI is not allowed to act before the game is in progress.
	// AI is not allowed to act after the game is finished.
	if (!GetAdministrator()->IsGameInProgress())
	{
		ProcessNonPlayableAction(executedAction);
		return;
	}
	// AI is allowed to access actions outside of the turn.
	ProcessPlayableAction(executedAction);
	
	// AI acts primarily during its respective turn.
	if (GetAdministrator()->CanEndTurn())
	{
		// AI can start making new decisions only once on its respective turn.
		// Prevents repeated nesting as it forces AI code to break.
		if (!InTurnPlay)
		{
			InTurnExecution();
		}
		return;
	}
	// AI is allowed to act outside of its turn, but it should be used with caution.
	else
	{
		// AI can make new decisions in response to other players play outside of its respective turn.
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

void AUDAIController::ProcessNonPlayableAction(const FUDActionData& executedAction)
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