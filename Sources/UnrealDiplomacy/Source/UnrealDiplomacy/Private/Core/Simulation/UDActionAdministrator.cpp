// Copyright Miroslav Valach

#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"

#pragma region Core
void UUDActionAdministrator::OnDataChanged(const FUDActionData& action)
{
	OnDataChangedEvent.Broadcast();
}

void UUDActionAdministrator::OnDataReloaded()
{
	OnDataReloadedEvent.Broadcast();
}

void UUDActionAdministrator::RequestAction(FUDActionData data)
{
	OnUserActionRequestedDelegate.ExecuteIfBound(data);
}

void UUDActionAdministrator::SetOverseeingState(TObjectPtr<UUDWorldState> overseeingState)
{
	if (IsOverseeingStatePresent())
	{
		UE_LOG(LogTemp, Log,
			TEXT("UUDActionAdministrator(%d): Multiple attempts to set overseeing state."),
			State->FactionPerspective);
		return;
	}
	State = overseeingState;
}
bool UUDActionAdministrator::IsOverseeingStatePresent()
{
	return IsValid(State);
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId)
{
	return FUDActionData(actionId, State->FactionPerspective);
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId, TArray<int32> optionalValues)
{
	return FUDActionData(actionId, State->FactionPerspective, optionalValues);
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId, TArray<int32> optionalValues, FString optionalString)
{
	return FUDActionData(actionId, State->FactionPerspective, optionalValues, optionalString);
}

FUDActionData UUDActionAdministrator::GetAction(int32 actionId, FString optionalString)
{
	return FUDActionData(actionId, State->FactionPerspective, optionalString);
}

FUDActionData UUDActionAdministrator::GetAcceptAction(int32 actionId, FUDActionData sourceAction)
{
	return FUDActionData::AsSuccessorOf(sourceAction, actionId);
}

FUDActionData UUDActionAdministrator::GetRejectAction(int32 actionId, FUDActionData sourceAction)
{
	return FUDActionData::AsSuccessorOf(sourceAction, actionId);
}

#pragma endregion

TObjectPtr<UUDMapState> UUDActionAdministrator::GetMapState()
{
	return State->Map;
}

#pragma region Lobby

TArray<FUDFactionMinimalInfo> UUDActionAdministrator::GetFactionList()
{
	TArray<FUDFactionMinimalInfo> factions = { };

	for (const auto& faction : State->Factions)
	{
		FUDFactionMinimalInfo newInfo = FUDFactionMinimalInfo(
			faction.Value->PlayerUniqueId,
			faction.Value->Name
		);
		factions.Add(newInfo);
	}

	return factions;
};

FString UUDActionAdministrator::GetLocalFactionName()
{
	return State->Factions[State->FactionPerspective]->Name;
}

#pragma endregion