// Copyright Miroslav Valach

#include "Core/Simulation/UDActionInterface.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDWorldGenerator.h"
#include "Core/Simulation/UDModifierManager.h"

bool IUDActionInterface::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Verifying...\n%s\n%s"), 
		world->PerspectivePlayerId, action.ToString(), ToString());
	
	bool isActionSame = action.ActionTypeId == GetId();
	bool isParameterCountSame = action.ValueParameters.Num() == GetParameterCount();
	return isActionSame && isParameterCountSame;
}	 

void IUDActionInterface::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Executing %s"), 
		world->PerspectivePlayerId, action.ToString());
}

void IUDActionInterface::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Reverting %s"), 
		world->PerspectivePlayerId, action.ToString());
}

int32 IUDActionInterface::GetId() const
{
	UUDGlobalData::InvalidActionId;
}

int32 IUDActionInterface::GetParameterCount() const
{
	UUDGlobalData::DefaultActionParameterCount;
}

bool IUDActionInterface::HasContinuations() const
{
	return false;
}

TArray<FUDActionData> IUDActionInterface::GetContinuations(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	return { };
}

bool IUDActionInterface::IsBackupRequired() const
{
	return false;
}

void IUDActionInterface::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	return;
}

void IUDActionInterface::SetWorldGenerator(TObjectPtr<UUDWorldGenerator> generator)
{
	return;
}

void IUDActionInterface::SetModifierManager(TObjectPtr<UUDModifierManager> manager)
{
	return;
}

FString IUDActionInterface::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	formatArgs.Add(TEXT("aid"), GetId());
	formatArgs.Add(TEXT("values"), GetParameterCount());

	FString formatted = FString::Format(TEXT("Executor[ID={aid}, OPTIONAL={values}]"), formatArgs);

	return formatted;
}

FUDActionPresentation IUDActionInterface::GetPresentation() const
{
	FUDActionPresentation presentation = FUDActionPresentation();

	presentation.Tags.Append({
		FUDActionPresentation::INVALID,
	});

	return presentation;
}


// TODO LIST
// TODO extend by adding more functions that do verify & check restriction
// UUDUnconditionalGiftAction is missing condition (action.InvokerPlayerId != action.TargetPlayerId)
// TODO CanExecute overloads that check if the action is still queued in PendingRequests.
// TODO Create action class that has function RemovePendingTargetRequest as default implementation.
// TODO reverting confirm or reject does not result in same action, if the action would be modified, e.g.
// only part of value is accepted as gift the original action would not be restorable. In case of adding
// new option that makes partial accept, confirm for that action should be separate and it should extend parameter
// count with the original and changed value.
// TODO update modifiers to use predicates to minimize code
// TODO consider that action like responses or deal creation could be using sequence of actions e.g. composite
// this would allow them to do only one thing, while allowing additional effects to take place via consequent actions.
// TODO reevaluate UniqueId and SourceUniqueId based on composite actions and revert. Currently UniqueId is never used as
// it's available always the same way SourceUniqueId is and SourceUniqueId is universal identifier for response actions.
// This makes SDID more suitable in current code base as main Id during creating entities, while UniqueId has no role
// other than synchronization of Client and Server in batch.


USTRUCT()
struct FUDActionPresentation
{
	GENERATED_BODY()
public:
	FUDActionPresentation() {}
	TSet<int32> Tags;
	// What is UI name
	FString Name;
	// What is UI desc
	FString Description;
	// What is UI icon
	FString Icon;
	// What is UI deal preview
	FString Preview;
	// What is UI deal final
	FString Final;
	static const int32 INVALID = -1;
	static const int32 VALID = 0;
};

void IUDActionInterface::AddPendingTargetRequest(FUDActionData action, int32 targetId, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Adding request(%d)"),
		world->PerspectivePlayerId,
		targetId,
		world->Players[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);

	// Item is added as key value pair.
	world->Players[targetId]->PendingRequests.Add(action.SourceUniqueId, action);

	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		world->PerspectivePlayerId,
		targetId,
		world->Players[targetId]->PendingRequests.Num()
	);
}

void IUDActionInterface::RemovePendingTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Deleting request(%d)"),
		world->PerspectivePlayerId, 
		targetId,
		world->Players[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);
	
	// Item is removed based on the key.
	world->Players[targetId]->PendingRequests.Remove(action.SourceUniqueId);

	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		world->PerspectivePlayerId,
		targetId,
		world->Players[targetId]->PendingRequests.Num()
	);
}

bool IUDActionInterface::IsPendingTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world) const
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Finding request(%d)"),
		world->PerspectivePlayerId,
		targetId,
		world->Players[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);

	// Item is found based on the key.
	if (world->Players[targetId]->PendingRequests.Contains(action.SourceUniqueId))
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Including request(%d)."),
			world->PerspectivePlayerId,
			targetId,
			world->Players[targetId]->PendingRequests.Num(),
			action.SourceUniqueId
		);
		return true;
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Did not found request(%d)"),
		world->PerspectivePlayerId,
		targetId,
		world->Players[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);
	return false;
}