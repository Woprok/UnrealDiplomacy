// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDDecisionAction.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"

int32 UUDDecisionAction::DecisionTypeToInteger(EUDDecisionType type)
{
	return static_cast<int32>(static_cast<uint8>(type));
}

EUDDecisionType UUDDecisionAction::IntegerToDecisionType(int32 type)
{
	if (0 > type || type > UINT8_MAX)
		return EUDDecisionType::Error;
	return static_cast<EUDDecisionType>(type);
}

void UUDDecisionAction::AddPendingTargetDecision(int32 targetId, int32 decisionId, FUDDecision decision, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) decisions. Adding decision(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num(),
		decisionId
	);

	// Item is added as key value pair.
	world->Factions[targetId]->PendingDecisions.Add(decisionId, decision);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) decisions."),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num()
	);
}

void UUDDecisionAction::RemovePendingTargetDecision(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) decisions. Deleting decision(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num(),
		decisionId
	);

	// Item is removed based on the key.
	world->Factions[targetId]->PendingDecisions.Remove(decisionId);

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) decisions."),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num()
	);
}

bool UUDDecisionAction::IsPendingTargetDecision(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world) const
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) decisions. Finding decision(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num(),
		decisionId
	);

	// Item is found based on the key.
	if (world->Factions[targetId]->PendingDecisions.Contains(decisionId))
	{
		UE_LOG(LogTemp, Log,
			TEXT("INSTANCE(%d): Player(%d) has (%d) decisions. Including decision(%d)."),
			world->FactionPerspective,
			targetId,
			world->Factions[targetId]->PendingDecisions.Num(),
			decisionId
		);
		return true;
	}

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) decisions. Did not found decision(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num(),
		decisionId
	);
	return false;
}

bool UUDDecisionAction::IsPendingInterchangeableTargetDecision(const FUDDecision& decision, int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world) const
{
	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) decisions. Finding decision(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num(),
		decisionId
	);

	// Item is found based on the key.
	bool hasSameActionQueued = false;

	for (const TPair<int32, FUDDecision>& key_action : world->Factions[targetId]->PendingDecisions)
	{
		if (key_action.Value.IsValueEqual(decision))
		{
			hasSameActionQueued = true;
			break;
		}
	}

	if (hasSameActionQueued)
	{
		UE_LOG(LogTemp, Log,
			TEXT("INSTANCE(%d): Player(%d) has (%d) decisions. Including decision(%d)."),
			world->FactionPerspective,
			targetId,
			world->Factions[targetId]->PendingDecisions.Num(),
			decisionId
		);
		return true;
	}

	UE_LOG(LogTemp, Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) decisions. Did not found decision(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingDecisions.Num(),
		decisionId
	);
	return false;
}

void UUDDecisionAction::MoveToPending(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world)
{
	// Get
	FUDDecision decision = world->Factions[targetId]->ResolvedDecisions[decisionId];
	// Remove
	world->Factions[targetId]->ResolvedDecisions.Remove(decisionId);
	// Add
	world->Factions[targetId]->PendingDecisions.Add(decisionId, decision);
}

void UUDDecisionAction::MoveToResolved(int32 targetId, int32 decisionId, TObjectPtr<UUDWorldState> world)
{
	// Get
	FUDDecision decision = world->Factions[targetId]->PendingDecisions[decisionId];
	// Remove
	world->Factions[targetId]->PendingDecisions.Remove(decisionId);
	// Add
	world->Factions[targetId]->ResolvedDecisions.Add(decisionId, decision);
}