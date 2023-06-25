// Copyright Miroslav Valach

#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionManager.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "Core/Simulation/UDModifierManager.h"

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

	resources.Add(FUDResourceInfo::GetReputation(faction->Resources[UD_RESOURCE_REPUTATION_ID]));
	resources.Add(FUDResourceInfo::GetGold(faction->Resources[UD_RESOURCE_GOLD_ID]));
	resources.Add(FUDResourceInfo::GetFood(faction->Resources[UD_RESOURCE_FOOD_ID]));
	resources.Add(FUDResourceInfo::GetMaterials(faction->Resources[UD_RESOURCE_MATERIALS_ID]));
	resources.Add(FUDResourceInfo::GetLuxuries(faction->Resources[UD_RESOURCE_LUXURIES_ID]));

	return resources;
}

TArray<FUDResourceMinimalInfo> UUDActionAdministrator::GetResourceItemList()
{
	TArray<FUDResourceMinimalInfo> resources = { };

	for (const auto& res : GetResourceList())
	{
		FUDResourceMinimalInfo resource;
		resource.Id = res.Id;
		resource.Name = res.Name;
		resources.Add(resource);
	}

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
	if (IsFactionPlayerControlled(tile->OwnerUniqueId))
	{
		tileInfo.FactionName = State->Factions[tile->OwnerUniqueId]->Name;
	}
	else
	{
		tileInfo.FactionName = TEXT("Gaia");
	}
	tileInfo.ResourceAmount = tile->ResourceStored;
	
	for (const auto& res : GetResourceList())
	{
		if (res.Id == tile->ResourceType)
			tileInfo.ResourceTypeName = res.Name;
	}
	
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
TArray<FUDActionMinimalInfo> UUDActionAdministrator::GetActionList()
{
	TArray<FUDActionMinimalInfo> actions = { };

	for (const auto& stratagem : ActionManager->FilterStratagems())
	{
		FUDActionMinimalInfo action;
		action.Id = stratagem.ActionId;
		action.Name = stratagem.Name;
		actions.Add(action);
	}

	return actions;
}

bool UUDActionAdministrator::HasTileParameter(const TSet<int32>& tags, int32 excludeTag)
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

bool UUDActionAdministrator::HasDealParameter(const TSet<int32>& tags, int32 excludeTag)
{
	TSet<int32> searchTags = {
		UD_ACTION_TAG_PARAMETER_DEAL
	};

	if (searchTags.Contains(excludeTag))
		return false;

	if (searchTags.Intersect(tags).Num() == 0)
		return false;

	return true;
}

bool UUDActionAdministrator::HasFactionParameter(const TSet<int32>& tags, int32 excludeTag)
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

bool UUDActionAdministrator::HasActionParameter(const TSet<int32>& tags, int32 excludeTag)
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

bool UUDActionAdministrator::HasResourceParameter(const TSet<int32>& tags, int32 excludeTag)
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

bool UUDActionAdministrator::HasValueParameter(const TSet<int32>& tags, int32 excludeTag)
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

bool UUDActionAdministrator::HasTextParameter(const TSet<int32>& tags, int32 excludeTag)
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

ParameterData UUDActionAdministrator::GetDealParameter(const TSet<int32>& tags)
{
	ParameterData data;
	FUDDealParameter parameter;

	parameter.Name = FText(LOCTEXT("Parameters", "Deal")).ToString();
	parameter.ToolTip = FText(LOCTEXT("Parameters", "Deal is required to be selected with this action.")).ToString();

	parameter.Options = GetDealList();

	data.Set<FUDDealParameter>(parameter);
	return data;
}

ParameterData UUDActionAdministrator::GetTileParameter(const TSet<int32>& tags)
{
	ParameterData data;
	FUDTileParameter parameter;

	parameter.Name = FText(LOCTEXT("Parameters", "Tile")).ToString();
	parameter.ToolTip = FText(LOCTEXT("Parameters", "Tile is required to be used with this action.")).ToString();

	parameter.Options = GetTileOptions();

	data.Set<FUDTileParameter>(parameter);
	return data;
}

ParameterData UUDActionAdministrator::GetFactionParameter(const TSet<int32>& tags)
{
	ParameterData data;
	FUDFactionParameter parameter;

	parameter.Name = FText(LOCTEXT("Parameters", "Faction")).ToString();
	parameter.ToolTip = FText(LOCTEXT("Parameters", "Faction is required to be selected with this action.")).ToString();

	parameter.Options = GetFactionList();

	data.Set<FUDFactionParameter>(parameter);
	return data;
}

ParameterData UUDActionAdministrator::GetActionParameter(const TSet<int32>& tags)
{
	ParameterData data;
	FUDActionParameter parameter;

	parameter.Name = FText(LOCTEXT("Parameters", "Action")).ToString();
	parameter.ToolTip = FText(LOCTEXT("Parameters", "Action is required to be selected for this action.")).ToString();

	parameter.Options = GetActionList();

	data.Set<FUDActionParameter>(parameter);
	return data;
}

ParameterData UUDActionAdministrator::GetResourceParameter(const TSet<int32>& tags)
{
	ParameterData data;
	FUDResourceParameter parameter;

	parameter.Name = FText(LOCTEXT("Parameters", "Resource")).ToString();
	parameter.ToolTip = FText(LOCTEXT("Parameters", "Resource is required to be chosen with this action.")).ToString();

	parameter.Options = GetResourceItemList();

	data.Set<FUDResourceParameter>(parameter);
	return data;
}

ParameterData UUDActionAdministrator::GetValueParameter(const TSet<int32>& tags)
{
	ParameterData data;
	FUDValueParameter parameter;

	if (tags.Contains(UD_ACTION_TAG_PARAMETER_VALUE_AMOUNT))
	{
		parameter.Name = FText(LOCTEXT("Parameters", "Amount")).ToString();
		parameter.ToolTip = FText(LOCTEXT("Parameters", "Amount is required to be chosen for this action.")).ToString();
	}
	else
	{
		parameter.Name = FText(LOCTEXT("Parameters", "Value")).ToString();
		parameter.ToolTip = FText(LOCTEXT("Parameters", "Value is required to be defined with this action.")).ToString();
	}

	parameter.MinValue = 1;
	parameter.MaxValue = 100;
	if (tags.Contains(UD_ACTION_TAG_PARAMETER_VALUE_SMALL_MAX))
	{
		parameter.MaxValue = 50;
	}

	data.Set<FUDValueParameter>(parameter);
	return data;
}

ParameterData UUDActionAdministrator::GetTextParameter(const TSet<int32>& tags)
{
	ParameterData data;
	FUDTextParameter parameter;

	parameter.Name = FText(LOCTEXT("Parameters", "Text")).ToString();
	parameter.ToolTip = FText(LOCTEXT("Parameters", "Text is required to be provided with this action.")).ToString();

	data.Set<FUDTextParameter>(parameter);
	return data;
}

// Note this is similiar to GetMessageContentArguments, in case of changing this change the other one to match.
FUDParameterListInfo UUDActionAdministrator::GetActionParameters(const TSet<int32>& tags, int32 excludeTag)
{
	FUDParameterListInfo parameters;
	parameters.OrderedType.Empty(0);
	parameters.OrderedData.Empty(0);
	// Good old if hell.
	// TODO create smarter system for parameters.
	if (HasDealParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Deal);
		parameters.OrderedData.Add(GetDealParameter(tags));
	}
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

FStringFormatArg UUDActionAdministrator::GetDealArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 dealId = data[startIndex++];
	return FStringFormatArg(State->Deals[dealId]->Name);
}

FStringFormatArg UUDActionAdministrator::GetFactionArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 factionId = data[startIndex++];
	return FStringFormatArg(State->Factions[factionId]->Name);
}

FStringFormatArg UUDActionAdministrator::GetTileArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 x = data[startIndex++];
	int32 y = data[startIndex++];
	return FStringFormatArg(State->Map->GetTile(FIntPoint(x, y))->Name);
}

FStringFormatArg UUDActionAdministrator::GetActionArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 actionId = data[startIndex++];
	return FStringFormatArg(ActionManager->GetAction(actionId)->GetPresentation().Name);
}

FStringFormatArg UUDActionAdministrator::GetResourceArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 resourceId = data[startIndex++];
	FString name = TEXT("");
	for (const auto& resource : GetResourceList())
	{
		if (resource.Id == resourceId)
		{
			name = resource.Name;
			break;
		}
	}
	return FStringFormatArg(name);
}

FStringFormatArg UUDActionAdministrator::GetValueArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 value = data[startIndex++];
	return FStringFormatArg(value);
}

FStringFormatArg UUDActionAdministrator::GetTextArgument(FString data)
{
	return FStringFormatArg(data);
}

// Note this is similiar to GetActionParameters, in case of changing this change the other one to match.
FStringFormatNamedArguments UUDActionAdministrator::GetMessageContentArguments(const TSet<int32>& tags, FUDActionData action)
{
	FStringFormatNamedArguments args;
	int32 startIndex = 0;

	args.Add(UD_PARAMETER_ARG_FACTION_INVOKER,
		FStringFormatArg(State->Factions[action.InvokerFactionId]->Name));

	if (HasDealParameter(tags, UD_INVALID_TAG_ID))
	{
		args.Add(UD_PARAMETER_ARG_DEAL,
			GetDealArgument(action.ValueParameters, startIndex));
	}
	if (HasFactionParameter(tags, UD_INVALID_TAG_ID))
	{
		args.Add(UD_PARAMETER_ARG_FACTION_TARGET,
			GetFactionArgument(action.ValueParameters, startIndex));
	}
	if (HasTileParameter(tags, UD_INVALID_TAG_ID))
	{
		args.Add(UD_PARAMETER_ARG_TILE,
			GetTileArgument(action.ValueParameters, startIndex));
	}
	if (HasActionParameter(tags, UD_INVALID_TAG_ID))
	{
		args.Add(UD_PARAMETER_ARG_ACTION,
			GetActionArgument(action.ValueParameters, startIndex));
	}
	if (HasResourceParameter(tags, UD_INVALID_TAG_ID))
	{
		args.Add(UD_PARAMETER_ARG_RESOURCE, 
			GetResourceArgument(action.ValueParameters, startIndex));
	}
	if (HasValueParameter(tags, UD_INVALID_TAG_ID))
	{
		args.Add(UD_PARAMETER_ARG_VALUE, 
			GetValueArgument(action.ValueParameters, startIndex));
	}
	if (HasTextParameter(tags, UD_INVALID_TAG_ID))
	{
		args.Add(UD_PARAMETER_ARG_TEXT, 
			GetTextArgument(action.TextParameter));
	}

	return args;
}

FString UUDActionAdministrator::GetFormattedMessageContent(FString formatString, const TSet<int32>& tags, FUDActionData action)
{
	return FString::Format(*formatString, GetMessageContentArguments(tags, action));
}

FUDMessageInfo UUDActionAdministrator::CreateMessageFromRequest(int32 requestId, FUDActionData action)
{
	FUDMessageInfo message = FUDMessageInfo();
	message.RequestId = requestId;
	FUDActionPresentation presentation = ActionManager->GetAction(action.ActionTypeId)->GetPresentation();
	message.ActionId = presentation.ActionId;
	message.Name = presentation.Name;
	message.AcceptId = presentation.AcceptActionId;
	message.RejectId = presentation.RejectActionId;
	message.Content = GetFormattedMessageContent(presentation.MessageContentFormat, presentation.Tags, action);
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

#pragma region Modifiers

FStringFormatNamedArguments UUDActionAdministrator::GetModifierContentArguments(const TSet<int32>& tags, FUDModifierData modifier)
{
	FStringFormatNamedArguments args;
	
	args.Add(UD_PARAMETER_ARG_FACTION_INVOKER,
		FStringFormatArg(State->Factions[modifier.InvokerId]->Name));
	args.Add(UD_PARAMETER_ARG_FACTION_TARGET,
		FStringFormatArg(State->Factions[modifier.TargetId]->Name));

	return args;
}

FString UUDActionAdministrator::GetFormattedModifierContent(FString formatString, const TSet<int32>& tags, FUDModifierData modifier)
{
	return FString::Format(*formatString, GetModifierContentArguments(tags, modifier));
}

TArray<FUDModifierInfo> UUDActionAdministrator::GetTileModifierList(FIntPoint tile)
{
	const auto& modifiers = State->Map->GetTile(tile)->Modifiers;
	return GetModifierList(modifiers);
}

TArray<FUDModifierInfo> UUDActionAdministrator::GetFactionModifierList(int32 factionId)
{
	const auto& modifiers = State->Factions[factionId]->Modifiers;
	return GetModifierList(modifiers);
}

TArray<FUDModifierInfo> UUDActionAdministrator::GetModifierList(const TArray<FUDModifierData>& modifiers)
{
	TArray<FUDModifierInfo> infos = { };
	const auto& modifierManager = ActionManager->GetModifierManager();
	for (const auto& mod : modifiers)
	{
		FUDModifierInfo info = FUDModifierInfo();
		info.Id = mod.ModifierTypeId;
		const auto& presentation = modifierManager->GetModifier(mod.ModifierTypeId)->GetPresentation();
		info.Name = presentation.Name;
		info.Description = GetFormattedModifierContent(presentation.ModifierDescriptionFormat, presentation.Tags, mod);
		infos.Add(info);
	}
	
	return infos;
}
#pragma endregion

#pragma region Deals

FUDDealInteractionInfo UUDActionAdministrator::GetAllLocalDeals()
{
	FUDDealInteractionInfo info;

	info.Active = { };
	info.History = { };

	for (const auto& deal : State->Deals)
	{
		// Ignore deals that we are not part of at the moment.
		if (!deal.Value->Participants.Contains(State->FactionPerspective))
			continue;
		if (deal.Value->DealSimulationResult == EUDDealSimulationResult::Opened)
		{
			// It's opened.
			FUDDealMinimalInfo dealInfo = FUDDealMinimalInfo();
			dealInfo.DealId = deal.Key;
			dealInfo.Name = deal.Value->Name;
			info.Active.Add(dealInfo);
		}
		else 
		{
			// It's resolved.
			FUDDealMinimalInfo dealInfo = FUDDealMinimalInfo();
			dealInfo.Name = deal.Value->Name;
			info.Active.Add(dealInfo);
		}
	}

	return info;
}

TArray<FUDChatMessageInfo> UUDActionAdministrator::GetDealChatHistory(int32 dealId)
{
	TArray<FUDChatMessageInfo> history = { };

	for (const auto& deal : State->Deals[dealId]->ChatHistory)
	{
		FUDChatMessageInfo info;
		info.FactionName = State->Factions[deal.AuthorId]->Name;
		info.Message = deal.Message;
		history.Add(info);
	}

	return history;
}

TArray<FUDDealMinimalInfo> UUDActionAdministrator::GetDealList()
{
	TArray<FUDDealMinimalInfo> deals = { };

	for (const auto& dealDetail : State->Deals)
	{
		FUDDealMinimalInfo deal;
		deal.DealId = dealDetail.Key;
		deal.Name = dealDetail.Value->Name;
		deals.Add(deal);
	}

	return deals;
}

#define LOCTEXT_NAMESPACE "DealStateConvertor"

FText UUDActionAdministrator::GetStateName(EUDDealSimulationState state, EUDDealSimulationResult result)
{
	switch (result)
	{
	case EUDDealSimulationResult::Opened:
		switch (state)
		{
		case EUDDealSimulationState::CreatingDraft:
			return FText(LOCTEXT("DealStateConvertor", "Draft Phase"));
			break;
		case EUDDealSimulationState::Assembling:
			return FText(LOCTEXT("DealStateConvertor", "Assemble Phase"));
			break;
		case EUDDealSimulationState::ExtendingDraft:
			return FText(LOCTEXT("DealStateConvertor", "Extension Phase"));
			break;
		case EUDDealSimulationState::DemandsAndRequests:
			return FText(LOCTEXT("DealStateConvertor", "Demands & Requests Phase"));
			break;
		case EUDDealSimulationState::Bidding:
			return FText(LOCTEXT("DealStateConvertor", "Bidding Phase"));
			break;
		case EUDDealSimulationState::FinalizingDraft:
			return FText(LOCTEXT("DealStateConvertor", "Final Check Phase"));
			break;
		case EUDDealSimulationState::Vote:
			return FText(LOCTEXT("DealStateConvertor", "Vote Phase"));
			break;
		case EUDDealSimulationState::Resolution:
			return FText(LOCTEXT("DealStateConvertor", "Resolution Phase"));
			break;
		}
		break;
	case EUDDealSimulationResult::Passed:
		return FText(LOCTEXT("DealStateConvertor", "Passed"));
		break;
	case EUDDealSimulationResult::Vetoed:
		return FText(LOCTEXT("DealStateConvertor", "Vetoed"));
		break;
	case EUDDealSimulationResult::Disassembled:
		return FText(LOCTEXT("DealStateConvertor", "Disassembled"));
		break;
	case EUDDealSimulationResult::Closed:
		return FText(LOCTEXT("DealStateConvertor", "Closed"));
		break;
	}
	return FText(LOCTEXT("DealStateConvertor", "Undefined Phase"));
}

#undef LOCTEXT_NAMESPACE

FUDDealInfo UUDActionAdministrator::GetDealInfo(int32 dealId)
{
	FUDDealInfo dealInfo = FUDDealInfo();
	dealInfo.IsModerator = State->Deals[dealId]->OwnerUniqueId == State->FactionPerspective;
	dealInfo.State = GetStateName(State->Deals[dealId]->DealSimulationState, State->Deals[dealId]->DealSimulationResult).ToString();
	dealInfo.ReadyCount = State->Deals[dealId]->IsReadyPlayerList.Num();
	dealInfo.VoteCount = State->Deals[dealId]->PositiveVotePlayerList.Num();
	dealInfo.ParticipantCount = State->Deals[dealId]->Participants.Num();
	dealInfo.LocalReady = State->Deals[dealId]->IsReadyPlayerList.Contains(State->FactionPerspective);
	dealInfo.LocalVote = State->Deals[dealId]->PositiveVotePlayerList.Contains(State->FactionPerspective);
	return dealInfo;
}
TArray<FUDDealFactionInfo> UUDActionAdministrator::GetDealParticipantList(int32 dealId)
{
	TArray<FUDDealFactionInfo> participants = { };

	for (const auto& member : State->Deals[dealId]->Participants)
	{
		FUDDealFactionInfo faction;
		faction.DealId = dealId;
		faction.FactionId = member;
		faction.FactionName = State->Factions[member]->Name;
		faction.IsInviteble = false;
		faction.IsReady = State->Deals[dealId]->IsReadyPlayerList.Contains(member);
		faction.IsYesVote = State->Deals[dealId]->PositiveVotePlayerList.Contains(member);
		participants.Add(faction);
	}

	return participants;
}

TArray<FUDDealFactionInfo> UUDActionAdministrator::GetDealInviteList(int32 dealId)
{
	TArray<FUDDealFactionInfo> inviteables = { };

	for (const auto& nonmember : State->Factions)
	{
		if (State->Deals[dealId]->Participants.Contains(nonmember.Key))
		{
			continue;
		}
		FUDDealFactionInfo faction;
		faction.DealId = dealId;
		faction.FactionId = nonmember.Key;
		faction.FactionName = nonmember.Value->Name;
		// revert of blocked list, only nonblocked can join
		faction.IsInviteble = !State->Deals[dealId]->BlockedParticipants.Contains(nonmember.Key);
		faction.IsReady = false;
		faction.IsYesVote = false;
		inviteables.Add(faction);
	}

	return inviteables;
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