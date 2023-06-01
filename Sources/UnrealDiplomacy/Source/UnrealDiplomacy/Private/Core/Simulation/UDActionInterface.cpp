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
		world->FactionPerspective, *action.ToString(), *ToString());
	
	bool isActionSame = action.ActionTypeId == GetId();
	bool isParameterCountSame = action.ValueParameters.Num() == GetParameterCount();
	return isActionSame && isParameterCountSame;
}	 

void IUDActionInterface::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Executing %s"), 
		world->FactionPerspective, *action.ToString());
}

void IUDActionInterface::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(LogTemp, Log, TEXT("INSTANCE(%d): Reverting %s"), 
		world->FactionPerspective, *action.ToString());
}

int32 IUDActionInterface::GetId() const
{
	return UUDGlobalData::InvalidActionId;
}

int32 IUDActionInterface::GetParameterCount() const
{
	return UUDGlobalData::DefaultActionParameterCount;
}

bool IUDActionInterface::HasContinuations() const
{
	return false;
}

TArray<FUDActionData> IUDActionInterface::GetContinuations(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
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
	presentation.ActionId = GetId();
	presentation.Tags.Append({
		FUDActionPresentation::INVALID,
	});

	return presentation;
}

void IUDActionInterface::AddPendingTargetRequest(FUDActionData action, int32 targetId, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Adding request(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);

	// Item is added as key value pair.
	world->Factions[targetId]->PendingRequests.Add(action.SourceUniqueId, action);

	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingRequests.Num()
	);
}

void IUDActionInterface::RemovePendingTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world)
{
	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) started with (%d) requests. Deleting request(%d)"),
		world->FactionPerspective, 
		targetId,
		world->Factions[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);
	
	// Item is removed based on the key.
	world->Factions[targetId]->PendingRequests.Remove(action.SourceUniqueId);

	UE_LOG(
		LogTemp, 
		Log,
		TEXT("INSTANCE(%d): Player(%d) ended with (%d) requests."),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingRequests.Num()
	);
}

bool IUDActionInterface::IsPendingTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world) const
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Finding request(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);

	// Item is found based on the key.
	if (world->Factions[targetId]->PendingRequests.Contains(action.SourceUniqueId))
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Including request(%d)."),
			world->FactionPerspective,
			targetId,
			world->Factions[targetId]->PendingRequests.Num(),
			action.SourceUniqueId
		);
		return true;
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Did not found request(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);
	return false;
}

bool IUDActionInterface::IsPendingInterchangeableTargetRequest(const FUDActionData& action, int32 targetId, TObjectPtr<UUDWorldState> world) const
{
	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Finding request(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);

	// Item is found based on the key.
	bool hasSameActionQueued = false;

	for (const TPair<int32, FUDActionData>& key_action : world->Factions[targetId]->PendingRequests)
	{
		if (key_action.Value.IsValueEqual(action))
		{
			hasSameActionQueued = true;
			break;
		}
	}

	if (hasSameActionQueued)
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Including request(%d)."),
			world->FactionPerspective,
			targetId,
			world->Factions[targetId]->PendingRequests.Num(),
			action.SourceUniqueId
		);
		return true;
	}

	UE_LOG(
		LogTemp,
		Log,
		TEXT("INSTANCE(%d): Player(%d) has (%d) requests. Did not found request(%d)"),
		world->FactionPerspective,
		targetId,
		world->Factions[targetId]->PendingRequests.Num(),
		action.SourceUniqueId
	);
	return false;
}