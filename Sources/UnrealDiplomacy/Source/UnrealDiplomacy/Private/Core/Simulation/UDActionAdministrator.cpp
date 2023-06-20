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
		if (IsFactionPlayerControlled(faction.Value->PlayerUniqueId))
		{
			FUDFactionMinimalInfo newInfo = FUDFactionMinimalInfo(
				faction.Value->PlayerUniqueId,
				faction.Value->Name
			);
			factions.Add(newInfo);
		}
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
TArray<FUDFactionInfo> UUDActionAdministrator::GetFactionInfoList()
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

FUDFactionMinimalInfo UUDActionAdministrator::GetFactionInfo(int32 factionId)
{
	const auto& faction = State->Factions[factionId];
	FUDFactionMinimalInfo factionInfo = FUDFactionMinimalInfo(
		faction->PlayerUniqueId,
		faction->Name
	);
	return factionInfo;
}

TArray<FUDFactionInteractionInfo> UUDActionAdministrator::GetFactionInteractionList()
{
	TArray<FUDFactionInteractionInfo> interactions = { };

	for (const auto& interaction : GetActionManager()->FilterFactionInteractions())
	{
		bool isAvailable = IsAvailableStratagem(interaction.Tags, interaction.ActionId);
		// continue if it's not available with next.
		if (!isAvailable)
			continue;

		FUDFactionInteractionInfo newInfo;
		newInfo.Name = interaction.Name;
		newInfo.ActionTypeId = interaction.ActionId;
		newInfo.Parameters = GetActionParameters(interaction.Tags, UD_ACTION_TAG_PARAMETER_FACTION);
		interactions.Add(newInfo);
	}

	return interactions;
}

#pragma endregion

#pragma region Resources
TArray<FUDResourceInfo> UUDActionAdministrator::GetResourceList()
{
	TArray<FUDResourceInfo> resources = { };

	resources.Add(FUDResourceInfo::GetReputation(0));
	resources.Add(FUDResourceInfo::GetGold(0));
	resources.Add(FUDResourceInfo::GetFood(0));
	resources.Add(FUDResourceInfo::GetMaterials(0));
	resources.Add(FUDResourceInfo::GetLuxuries(0));

	return resources;
}

TArray<FUDResourceInfo> UUDActionAdministrator::GetLocalFactionResourceList()
{
	TArray<FUDResourceInfo> resources = { };

	TObjectPtr<UUDFactionState> faction = State->Factions[State->FactionPerspective];

	resources.Add(FUDResourceInfo::GetReputation(faction->ResourceReputation));
	resources.Add(FUDResourceInfo::GetGold(faction->ResourceGold));
	resources.Add(FUDResourceInfo::GetFood(faction->ResourceFood));
	resources.Add(FUDResourceInfo::GetMaterials(faction->ResourceMaterials));
	resources.Add(FUDResourceInfo::GetLuxuries(faction->ResourceLuxuries));

	return resources;
}
#pragma endregion

#pragma region Gameplay
bool UUDActionAdministrator::IsGamePlayed()
{
	if (State->WorldSimulationState == EUDWorldSimulationState::Simulating)
		return true;
	return false;
}
#pragma endregion

#pragma region Turn
FUDRegencyTurnInfo UUDActionAdministrator::GetRegencyTurnInfo()
{
	FUDRegencyTurnInfo newInfo;

	int32 regencyFaction = State->TurnData.RegentFaction;
	newInfo.IsRegent = regencyFaction == State->FactionPerspective;
	newInfo.RegentFactionName = State->Factions[regencyFaction]->Name;
	newInfo.Turn = State->TurnData.Turn;
	newInfo.CurrentFinished = State->TurnData.TurnFinishedFactions.Num();
	newInfo.MaximumFinished = State->TurnData.FactionTurnOrder.Num();
	newInfo.IsFinished = State->TurnData.TurnFinishedFactions.Contains(State->FactionPerspective);
	return newInfo;
}

bool UUDActionAdministrator::IsIntermezzo()
{
	return State->TurnData.IsIntermezzo;
}

bool UUDActionAdministrator::CanFinishTurn()
{
	return State->TurnData.TurnFinishedFactions.Contains(State->FactionPerspective);
}
#pragma endregion

#pragma region Diplomacy & Throne
FUDThroneInfo UUDActionAdministrator::GetThroneInfo()
{
	FUDThroneInfo info;
	
	info.State = EUDThroneState::Empty;
	info.CanInteract = true;
	// Name is not changed for empty, it should never be needed.

	if (State->ImperialThrone.Ruler == State->FactionPerspective)
	{
		info.State = EUDThroneState::Claimer;
		info.CanInteract = true;
		info.FactionName = State->Factions[State->ImperialThrone.Ruler]->Name;
	}
	else if (IsFactionPlayerControlled(State->ImperialThrone.Ruler))
	{
		info.State = EUDThroneState::Usurper;
		info.CanInteract = false;
		info.FactionName = State->Factions[State->ImperialThrone.Ruler]->Name;
	}

	return info;
}

FUDAvailableDiplomacyInfo UUDActionAdministrator::GetDiplomacyInfo()
{
	FUDAvailableDiplomacyInfo info;

	info.DealCount = GetActiveDealParticipationCount();
	info.MessageCount = GetUnresolvedMessagesCount();

	return info;
}

int32 UUDActionAdministrator::GetActiveDealParticipationCount()
{
	int32 participating = 0;
	for (const auto& deal : State->Deals)
	{
		if (deal.Value->Participants.Contains(State->FactionPerspective))
		{
			participating++;
		}
	}
	return participating;
}

int32 UUDActionAdministrator::GetUnresolvedMessagesCount()
{
	return State->Factions[State->FactionPerspective]->PendingRequests.Num();
}

#pragma endregion

#pragma region Tile & Map Interaction

TObjectPtr<UUDMapState> UUDActionAdministrator::GetMapState()
{
	return State->Map;
}

TArray<FUDTileInteractionInfo> UUDActionAdministrator::GetTileInteractionList()
{
	TArray<FUDTileInteractionInfo> interactions = { };

	for (const auto& interaction : GetActionManager()->FilterTileInteractions())
	{
		bool isAvailable = IsAvailableStratagem(interaction.Tags, interaction.ActionId);
		// continue if it's not available with next.
		if (!isAvailable)
			continue;

		FUDTileInteractionInfo newInfo;
		newInfo.Name = interaction.Name;
		newInfo.ActionTypeId = interaction.ActionId;
		newInfo.Parameters = GetActionParameters(interaction.Tags, UD_ACTION_TAG_PARAMETER_TILE);
		interactions.Add(newInfo);
	}

	return interactions;
}

FUDTileInfo UUDActionAdministrator::GetTileInfo(FIntPoint position)
{
	const auto& tile = State->Map->GetTile(position);
	FUDTileInfo tileInfo;
	tileInfo.Position = tile->Position;
	tileInfo.Name = tile->Name;
	tileInfo.FactionId	= tile->OwnerUniqueId;
	tileInfo.FactionName = State->Factions[tile->OwnerUniqueId]->Name;
	return tileInfo;
}

TArray<FUDTileMinimalInfo> UUDActionAdministrator::GetTileOptions()
{
	TArray<FUDTileMinimalInfo> tiles = { };

	for (const auto& tile : State->Map->Tiles)
	{
		FUDTileMinimalInfo info = FUDTileMinimalInfo();
		
		info.Position = tile->Position;
		info.Name = tile->Name;

		tiles.Add(info);
	}

	return tiles;
}

#pragma endregion

#pragma region Parameters
#define LOCTEXT_NAMESPACE "Parameters"

bool UUDActionAdministrator::HasTileParameter(TSet<int32> tags, int32 excludeTag)
{
	TSet<int32> searchTags = {
		UD_ACTION_TAG_PARAMETER_TILE
	};

	if (searchTags.Contains(excludeTag))
		return false;

	if (searchTags.Intersect(tags).Num() == 0)
		return false;

	return true;
}

bool UUDActionAdministrator::HasFactionParameter(TSet<int32> tags, int32 excludeTag)
{
	TSet<int32> searchTags = {
		UD_ACTION_TAG_PARAMETER_FACTION
	};

	if (searchTags.Contains(excludeTag))
		return false;

	if (searchTags.Intersect(tags).Num() == 0)
		return false;

	return true;
}

bool UUDActionAdministrator::HasActionParameter(TSet<int32> tags, int32 excludeTag)
{
	TSet<int32> searchTags = {
		UD_ACTION_TAG_PARAMETER_ACTION
	};

	if (searchTags.Contains(excludeTag))
		return false;

	if (searchTags.Intersect(tags).Num() == 0)
		return false;

	return true;
}

bool UUDActionAdministrator::HasResourceParameter(TSet<int32> tags, int32 excludeTag)
{
	TSet<int32> searchTags = {
		UD_ACTION_TAG_PARAMETER_RESOURCE
	};

	if (searchTags.Contains(excludeTag))
		return false;

	if (searchTags.Intersect(tags).Num() == 0)
		return false;

	return true;
}

bool UUDActionAdministrator::HasValueParameter(TSet<int32> tags, int32 excludeTag)
{
	TSet<int32> searchTags = {
		UD_ACTION_TAG_PARAMETER_VALUE
	};

	if (searchTags.Contains(excludeTag))
		return false;

	if (searchTags.Intersect(tags).Num() == 0)
		return false;

	return true;
}

bool UUDActionAdministrator::HasTextParameter(TSet<int32> tags, int32 excludeTag)
{
	TSet<int32> searchTags = {
		UD_ACTION_TAG_PARAMETER_TEXT
	};

	if (searchTags.Contains(excludeTag))
		return false;

	if (searchTags.Intersect(tags).Num() == 0)
		return false;

	return true;
}

ParameterData UUDActionAdministrator::GetTileParameter(TSet<int32> tags)
{
	ParameterData data;
	FUDTileParameter tile;

	tile.Name = FText(LOCTEXT("Parameters", "Tile")).ToString();
	tile.ToolTip = FText(LOCTEXT("Parameters", "Tile is required to be used with this action.")).ToString();

	tile.Options = GetTileOptions();

	data.Set<FUDTileParameter>(tile);
	return data;
}

ParameterData UUDActionAdministrator::GetFactionParameter(TSet<int32> tags)
{
	ParameterData data;
	FUDFactionParameter tile;

	tile.Name = FText(LOCTEXT("Parameters", "Faction")).ToString();
	tile.ToolTip = FText(LOCTEXT("Parameters", "Faction is required to be selected with this action.")).ToString();

	data.Set<FUDFactionParameter>(tile);
	return data;
}

ParameterData UUDActionAdministrator::GetActionParameter(TSet<int32> tags)
{
	ParameterData data;
	FUDActionParameter tile;

	tile.Name = FText(LOCTEXT("Parameters", "Action")).ToString();
	tile.ToolTip = FText(LOCTEXT("Parameters", "Action is required to be selected for this action.")).ToString();

	data.Set<FUDActionParameter>(tile);
	return data;
}

ParameterData UUDActionAdministrator::GetResourceParameter(TSet<int32> tags)
{
	ParameterData data;
	FUDResourceParameter tile;

	tile.Name = FText(LOCTEXT("Parameters", "Resource")).ToString();
	tile.ToolTip = FText(LOCTEXT("Parameters", "Resource is required to be chosen with this action.")).ToString();

	data.Set<FUDResourceParameter>(tile);
	return data;
}

ParameterData UUDActionAdministrator::GetValueParameter(TSet<int32> tags)
{
	ParameterData data;
	FUDValueParameter tile;

	if (tags.Contains(UD_ACTION_TAG_PARAMETER_VALUE_AMOUNT))
	{
		tile.Name = FText(LOCTEXT("Parameters", "Amount")).ToString();
		tile.ToolTip = FText(LOCTEXT("Parameters", "Amount is required to be chosen for this action.")).ToString();
	}
	else
	{
		tile.Name = FText(LOCTEXT("Parameters", "Value")).ToString();
		tile.ToolTip = FText(LOCTEXT("Parameters", "Value is required to be defined with this action.")).ToString();
	}

	tile.MinValue = 1;
	tile.MaxValue = 100;
	if (tags.Contains(UD_ACTION_TAG_PARAMETER_VALUE_SMALL_MAX))
	{
		tile.MaxValue = 50;
	}

	data.Set<FUDValueParameter>(tile);
	return data;
}

ParameterData UUDActionAdministrator::GetTextParameter(TSet<int32> tags)
{
	ParameterData data;
	FUDTextParameter tile;

	tile.Name = FText(LOCTEXT("Parameters", "Text")).ToString();
	tile.ToolTip = FText(LOCTEXT("Parameters", "Text is required to be provided with this action.")).ToString();

	data.Set<FUDTextParameter>(tile);
	return data;
}

FUDParameterListInfo UUDActionAdministrator::GetActionParameters(TSet<int32> tags, int32 excludeTag)
{
	FUDParameterListInfo parameters;
	parameters.OrderedType.Empty(0);
	parameters.OrderedData.Empty(0);
	// Good old if hell.
	// TODO create smarter system for parameters.
	if (HasFactionParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Faction);
		parameters.OrderedData.Add(GetFactionParameter(tags));
	}
	if (HasTileParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Tile);
		parameters.OrderedData.Add(GetTileParameter(tags));
	}
	if (HasActionParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Action);
		parameters.OrderedData.Add(GetActionParameter(tags));
	}
	if (HasResourceParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Resource);
		parameters.OrderedData.Add(GetResourceParameter(tags));
	}
	if (HasValueParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Value);
		parameters.OrderedData.Add(GetValueParameter(tags));
	}
	if (HasTextParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Text);
		parameters.OrderedData.Add(GetTextParameter(tags));
	}

	return parameters;
}

#undef LOCTEXT_NAMESPACE
#pragma endregion

#pragma region Messages & Request Interaction

FUDMessageInfo UUDActionAdministrator::CreateMessageFromRequest(int32 requestId, FUDActionData action)
{
	FUDMessageInfo message = FUDMessageInfo();
	
	message.RequestId = requestId;

	return message;
}

FUDActionData UUDActionAdministrator::GetPendingRequest(int32 pendingRequestId)
{
	return State->Factions[State->FactionPerspective]->PendingRequests[pendingRequestId];
}

FUDMessageInteractionInfo UUDActionAdministrator::GetAllLocalRequests()
{
	FUDMessageInteractionInfo info = FUDMessageInteractionInfo();
	info.Messages = { };

	for (const auto& request : State->Factions[State->FactionPerspective]->PendingRequests)
	{
		info.Messages.Add(CreateMessageFromRequest(request.Key, request.Value));
	}

	return info;
}
#pragma endregion

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

bool UUDActionAdministrator::IsAvailableStratagem(TSet<int32> tags, int32 actionId)
{
	if (tags.Contains(UD_ACTION_TAG_STRATAGEM))
		return State->Factions[State->FactionPerspective]->StratagemOptions.Contains(actionId);
	// Default is true as anything that was not defined as stratagem is always available to player.
	return true;
}