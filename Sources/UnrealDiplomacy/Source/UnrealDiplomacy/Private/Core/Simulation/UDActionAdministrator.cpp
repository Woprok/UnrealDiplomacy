// Copyright Miroslav Valach

#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionManager.h"
#include "Core/Simulation/UDWorldState.h"

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

TObjectPtr<UUDActionManager> UUDActionAdministrator::GetActionManager()
{
	if (ActionManager == nullptr)
	{
		InitializeActionManager();
	}
	return ActionManager;
}

void UUDActionAdministrator::InitializeActionManager()
{
	UE_LOG(LogTemp, Log, TEXT("UUDActionAdministrator: Creating manager for referencing Actions."));
	ActionManager = NewObject<UUDActionManager>(this);
	ActionManager->Initialize();
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

int32 UUDActionAdministrator::GetSettingsAICount()
{
	return State->Settings.AICount;
}

int32 UUDActionAdministrator::GetSettingsMapSeed()
{
	return State->Settings.MapSeed;
}

int32 UUDActionAdministrator::GetSettingsMapWidth()
{
	return State->Settings.MapWidth;
}

int32 UUDActionAdministrator::GetSettingsMapHeight()
{
	return State->Settings.MapHeight;
}

int32 UUDActionAdministrator::GetSettingsStratagemPoints()
{
	return State->Settings.StratagemPoints;
}

int32 UUDActionAdministrator::GetStratagemCostFromTags(TSet<int32> tags)
{
	if (tags.Contains(UD_ACTION_TAG_STRATAGEM_COST_1))
		return 1;
	if (tags.Contains(UD_ACTION_TAG_STRATAGEM_COST_2))
		return 2;
	if (tags.Contains(UD_ACTION_TAG_STRATAGEM_COST_3))
		return 3;
	if (tags.Contains(UD_ACTION_TAG_STRATAGEM_COST_4))
		return 4;
	if (tags.Contains(UD_ACTION_TAG_STRATAGEM_COST_5))
		return 5;
	return 0;
}

bool UUDActionAdministrator::IsStratagemTakeable(FUDStratagemPickableInfo stratagem)
{
	return GetLocalStratagemPointsLeft() >= stratagem.Cost;
}

int32 UUDActionAdministrator::GetLocalStratagemPointsLeft()
{
	int32 total = GetSettingsStratagemPoints();
	int32 costUsed = 0;
	for (const auto& stratagem : State->Factions[State->FactionPerspective]->StratagemOptions)
	{
		TSet<int32> tags = GetActionManager()->GetSpecified(stratagem).Tags;
		costUsed += GetStratagemCostFromTags(tags);
	}
	return total - costUsed;
}

TArray<FUDStratagemPickableInfo> UUDActionAdministrator::GetStratagemsList()
{
	TArray<FUDStratagemPickableInfo> stratagemList = { };
	
	for (const auto& stratagem : GetActionManager()->FilterStratagems())
	{
		bool isSelected = State->Factions[State->FactionPerspective]->StratagemOptions.Contains(stratagem.ActionId);
		stratagemList.Add(
			FUDStratagemPickableInfo(stratagem.ActionId, stratagem.Name, GetStratagemCostFromTags(stratagem.Tags), isSelected)
		);
	}

	return stratagemList;
}

EUDGameStateInfo UUDActionAdministrator::GetMatchStateInfo()
{
	switch (State->WorldSimulationState)
	{
	case EUDWorldSimulationState::Preparing:
		return EUDGameStateInfo::Lobby;
	case EUDWorldSimulationState::Simulating:
		return EUDGameStateInfo::Match;
	case EUDWorldSimulationState::Finished:
		return EUDGameStateInfo::Scoreboard;
	default:
		unimplemented();
		break;
	}
	return EUDGameStateInfo::Lobby;
}
#pragma endregion

#pragma region GameOver
bool UUDActionAdministrator::IsGameOver()
{
	if (State->WorldSimulationState == EUDWorldSimulationState::Finished)
		return true;
	return false;
}

FUDGameOverInfo UUDActionAdministrator::GetGameOverInfo()
{
	FUDGameOverInfo gameOverInfo;

	int32 winnerId = State->ImperialThrone.Ruler;
	gameOverInfo.IsWinner = winnerId == State->FactionPerspective;
	gameOverInfo.WinnerFactionId = winnerId;
	gameOverInfo.WinnerFactionName = State->Factions[winnerId]->Name;

	return gameOverInfo;
}
#pragma endregion

#pragma region Faction Interaction
TArray<FUDFactionInfo> UUDActionAdministrator::GetFactionInteractionList()
{
	TArray<FUDFactionInfo> factions = { };

	for (const auto& faction : State->Factions)
	{
		if (IsFactionPlayerControlled(faction.Value->PlayerUniqueId))
		{
			FUDFactionInfo newInfo = FUDFactionInfo(
				faction.Value->PlayerUniqueId,
				faction.Value->Name,
				faction.Value->Controller == EUDFactionController::Player
			);
			factions.Add(newInfo);
		}
	}

	return factions;
};
#pragma endregion

TObjectPtr<UUDMapState> UUDActionAdministrator::GetMapState()
{
	return State->Map;
}

bool UUDActionAdministrator::IsLocalFactionPlayer()
{
	bool notGaia = State->FactionPerspective != UUDGlobalData::GaiaFactionId;
	bool notObserver = State->FactionPerspective != UUDGlobalData::ObserverFactionId;
	return notGaia && notObserver;
}

bool UUDActionAdministrator::IsFactionPlayerControlled(int32 factionId)
{
	bool notGaia = factionId != UUDGlobalData::GaiaFactionId;
	bool notObserver = factionId != UUDGlobalData::ObserverFactionId;
	return notGaia && notObserver;
}