// Copyright Miroslav Valach

#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/UDGlobalData.h"
#include "Core/UDGameInstance.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/UDSettingManager.h"
#include "Core/Simulation/UDActionManager.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDModifierData.h"
#include "Core/Simulation/UDModifierInterface.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/UDActionInterface.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDResourceInterface.h"

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

TWeakObjectPtr<UUDActionManager> UUDActionAdministrator::GetActionManager()
{
	return ActionManager;
}

void UUDActionAdministrator::SetManagers(TWeakObjectPtr<UUDSettingManager> settingManager, TWeakObjectPtr<UUDActionManager> actionManager)
{
	SettingManager = settingManager;
	ActionManager = actionManager;
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

FUDActionData UUDActionAdministrator::ReverseActionInvokerAndTarget(FUDActionData actionData)
{
	int32 newTarget = actionData.InvokerFactionId;
	// new Invoker is original Target
	actionData.InvokerFactionId = actionData.ValueParameters[0];
	actionData.ValueParameters[0] = newTarget;
	return actionData;
}

#include "Core/Simulation/Actions/UDDecisionActionCreate.h"
#include "Core/Simulation/Actions/UDDecisionActionConfirm.h"
#include "Core/Simulation/Actions/UDDecisionActionDecline.h"

FUDActionData UUDActionAdministrator::GetDecisionAction(int32 targetId, EUDDecisionType type, FUDActionData requestedAction)
{
	TArray<int32> parameters = { targetId, UUDDecisionAction::DecisionTypeToInteger(type) };
	parameters.Append(FUDActionData::ToValues(requestedAction));
	return FUDActionData(UUDDecisionActionCreate::ActionTypeId, State->FactionPerspective, parameters);
}

FUDActionData UUDActionAdministrator::GetConfirmAction(int32 decisionId)
{
	return FUDActionData(UUDDecisionActionConfirm::ActionTypeId, State->FactionPerspective, { decisionId });
}

FUDActionData UUDActionAdministrator::GetDeclineAction(int32 decisionId)
{
	return FUDActionData(UUDDecisionActionDecline::ActionTypeId, State->FactionPerspective, { decisionId });
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
	return CreateFactionInteractionList(GetActionManager()->FilterFactionInteractions(UD_ACTION_TAG_DECISION_DIRECT));
}

TArray<FUDFactionInteractionInfo> UUDActionAdministrator::GetFactionOfferList()
{
	return CreateFactionInteractionList(GetActionManager()->FilterFactionInteractions(UD_ACTION_TAG_DECISION_OFFER));
}

TArray<FUDFactionInteractionInfo> UUDActionAdministrator::GetFactionRequestList()
{
	return CreateFactionInteractionList(GetActionManager()->FilterFactionInteractions(UD_ACTION_TAG_DECISION_REQUEST));
}

TArray<FUDFactionInteractionInfo> UUDActionAdministrator::GetFactionDemandList()
{
	return CreateFactionInteractionList(GetActionManager()->FilterFactionInteractions(UD_ACTION_TAG_DECISION_DEMAND));
}

TArray<FUDFactionInteractionInfo> UUDActionAdministrator::GetFactionConsequenceList()
{
	return CreateFactionInteractionList(GetActionManager()->FilterFactionInteractions(UD_ACTION_TAG_DECISION_CONSEQUENCE));
}

TArray<FUDFactionInteractionInfo> UUDActionAdministrator::CreateFactionInteractionList(TArray<FUDActionPresentation>&& availableActions)
{
	TArray<FUDFactionInteractionInfo> interactions = { };

	for (const auto& interaction : availableActions)
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

#include "Core/Simulation/Actions/UDDecisionActionConsequenceSelect.h"

TArray<FUDPolicySelectItemInfo> UUDActionAdministrator::GetConsequencePolicyList()
{
	TArray<FUDPolicySelectItemInfo> policies = { };

	for (const auto& interaction : GetActionManager()->FilterFactionInteractions(UD_ACTION_TAG_DECISION_CONSEQUENCE))
	{
		bool isAvailable = IsAvailableStratagem(interaction.Tags, interaction.ActionId);
		// continue if it's not available with next.
		if (!isAvailable)
			continue;

		FUDPolicySelectItemInfo newPolicyAction;
		newPolicyAction.Name = interaction.Name;
		newPolicyAction.SelectPolicyActionId = UUDDecisionActionConsequenceSelect::ActionTypeId;
		newPolicyAction.PolicyId = interaction.ActionId;
		newPolicyAction.IsSelected = State->Factions[State->FactionPerspective]->DecisionDemandPolicy == interaction.ActionId;
		policies.Add(newPolicyAction);
	}

	return policies;
}

#pragma endregion

#pragma region Resources
TArray<FUDResourceInfo> UUDActionAdministrator::GetResourceList()
{
	TArray<FUDResourceInfo> resources = { };

	for (const auto& res : ActionManager->GetResourceManager()->FilterResources())
	{
		resources.Add(FUDResourceInfo(res.ResourceId, res.Name, 0));
	}

	return resources;
}

TArray<FUDResourceInfo> UUDActionAdministrator::GetLocalFactionResourceList()
{
	TArray<FUDResourceInfo> resources = { };

	TObjectPtr<UUDFactionState> faction = State->Factions[State->FactionPerspective];

	for (const auto& res : ActionManager->GetResourceManager()->FilterResources())
	{
		int32 amount = ActionManager->GetResourceManager()->GetCurrent(faction, res.ResourceId);
		resources.Add(FUDResourceInfo(res.ResourceId, res.Name, amount));
	}

	return resources;
}

UMaterialInterface* UUDActionAdministrator::GetIcon(int32 resourceId)
{
	return ActionManager->GetResourceManager()->GetIcon(resourceId);
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
	newInfo.MaxTurn = SettingManager->MaxTurnCount;
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
	return !State->TurnData.TurnFinishedFactions.Contains(State->FactionPerspective);
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
		if (deal.Value->DealSimulationResult == EUDDealSimulationResult::Opened &&
			deal.Value->Participants.Contains(State->FactionPerspective))
		{
			participating++;
		}
	}
	return participating;
}

int32 UUDActionAdministrator::GetUnresolvedMessagesCount()
{
	return State->Factions[State->FactionPerspective]->PendingDecisions.Num();
}

#pragma endregion

#pragma region Tile & Map Interaction

bool UUDActionAdministrator::IsMapStatePresent()
{
	return State->Map != nullptr;
}

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

// Note this is similiar to GetPresentationContentArguments, in case of changing this change the other one to match.
FUDParameterListInfo UUDActionAdministrator::GetActionParameters(const TSet<int32>& tags, int32 excludeTag)
{
	FUDParameterListInfo parameters;
	parameters.OrderedType.Empty(0);
	parameters.OrderedData.Empty(0);
	// TODO create smarter system for parameters.
	// This determines only if it can supply named parameter for displaying.
	// Deal parameter has only one use and it will need some tweaking if it ever becomes editable.
	if (HasDealParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::Deal);
		parameters.OrderedData.Add(GetDealParameter(tags));
	}
	// Following are standard parameters...
	if (HasFactionParameter(tags, excludeTag))
	{
		parameters.OrderedType.Add(EUDParameterType::FactionTarget);
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
	if (dealId == -1)
		return FStringFormatArg(TEXT("INVALID"));
	return FStringFormatArg(State->Deals[dealId]->Name);
}

FStringFormatArg UUDActionAdministrator::GetFactionArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 factionId = data[startIndex++];
	if (factionId == -1)
		return FStringFormatArg(TEXT("INVALID"));
	return FStringFormatArg(State->Factions[factionId]->Name);
}

FStringFormatArg UUDActionAdministrator::GetTileArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 x = data[startIndex++];
	int32 y = data[startIndex++];
	if (x == -1 || y == -1)
		return FStringFormatArg(TEXT("INVALID"));
	return FStringFormatArg(State->Map->GetTile(FIntPoint(x, y))->Name);
}

FStringFormatArg UUDActionAdministrator::GetActionArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 actionId = data[startIndex++];
	// -1 is fine as invalid action is default executor.
	return FStringFormatArg(ActionManager->GetAction(actionId)->GetPresentation().Name);
}

FStringFormatArg UUDActionAdministrator::GetResourceArgument(const TArray<int32>& data, int32& startIndex)
{
	int32 resourceId = data[startIndex++];
	FString name = TEXT("INVALID");
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
	// This is fine to return negative number.
	return FStringFormatArg(value);
}

FStringFormatArg UUDActionAdministrator::GetTextArgument(FString data)
{
	return FStringFormatArg(data);
}

// Note this is similiar to GetActionParameters, in case of changing this change the other one to match.
FStringFormatNamedArguments UUDActionAdministrator::GetPresentationContentArguments(const TSet<int32>& tags, FUDActionData action)
{
	FStringFormatNamedArguments args;
	int32 startIndex = 0;

	if (action.InvokerFactionId == -1)
	{
		args.Add(UD_PARAMETER_ARG_FACTION_INVOKER, FStringFormatArg(TEXT("INVALID")));
	}
	else 
	{
		args.Add(UD_PARAMETER_ARG_FACTION_INVOKER, FStringFormatArg(State->Factions[action.InvokerFactionId]->Name));
	}		

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

FString UUDActionAdministrator::GetFormattedPresentationString(FString formatString, const TSet<int32>& tags, FUDActionData action)
{
	return FString::Format(*formatString, GetPresentationContentArguments(tags, action));
}

#define LOCTEXT_NAMESPACE "DecisionTypeConverter"
FText UUDActionAdministrator::GetFormattedDecisionType(EUDDecisionType type)
{
	switch (type)
	{
	case EUDDecisionType::Gift:
		return FText(LOCTEXT("DecisionTypeConverter", "Gift"));
		break;
	case EUDDecisionType::Offer:
		return FText(LOCTEXT("DecisionTypeConverter", "Offer"));
		break;
	case EUDDecisionType::Request:
		return FText(LOCTEXT("DecisionTypeConverter", "Request"));
		break;
	case EUDDecisionType::Demand:
		return FText(LOCTEXT("DecisionTypeConverter", "Demand"));
		break;
	case EUDDecisionType::Error:
	default:
		break;
	}
	return FText(LOCTEXT("DecisionTypeConverter", "Undefined"));
}
#undef LOCTEXT_NAMESPACE

#define LOCTEXT_NAMESPACE "CreateEmptyDecision"
FUDMessageContentInfo UUDActionAdministrator::CreateEmptyMessageContent()
{
	FUDMessageContentInfo content = FUDMessageContentInfo();
	content.Name = FText(LOCTEXT("CreateEmptyDecision", "Undefined Action")).ToString();
	content.Content = FText(LOCTEXT("CreateEmptyDecision", "No action is expected.")).ToString();
	return content;
}
#undef LOCTEXT_NAMESPACE

FUDMessageContentInfo UUDActionAdministrator::CreateMessageContent(FUDActionData action)
{
	FUDMessageContentInfo content = FUDMessageContentInfo();
	FUDActionPresentation presentation = ActionManager->GetAction(action.ActionTypeId)->GetPresentation();
	content.ActionTypeId = action.ActionTypeId;
	content.Name = presentation.Name;
	content.Content = GetFormattedPresentationString(presentation.MessageContentFormat, presentation.Tags, action);
	return content;
}

FUDMessageInfo UUDActionAdministrator::CreateMessageFromRequest(int32 decisionId, FUDDecision decision)
{
	FUDMessageInfo message = FUDMessageInfo();
	message.DecisionId = decisionId;
	message.Content = CreateMessageContent(decision.ConfirmAction);
	message.Type = GetFormattedDecisionType(decision.Type).ToString();

	if (decision.HasDecline)
	{
		message.AdditionalContent = CreateMessageContent(decision.DeclineAction);
		message.HasReject = true;
	}
	else
	{
		message.AdditionalContent = CreateEmptyMessageContent();
		message.HasReject = false;
	}
	
	message.HasChoices = decision.Type != EUDDecisionType::Gift;

	return message;
}

FUDActionData UUDActionAdministrator::GetPendingRequest(int32 pendingDecisionId)
{
	return State->Factions[State->FactionPerspective]->PendingDecisions[pendingDecisionId].ConfirmAction;
}

FUDMessageInteractionInfo UUDActionAdministrator::GetAllLocalRequests()
{
	FUDMessageInteractionInfo info = FUDMessageInteractionInfo();
	info.Messages = { };

	for (const auto& request : State->Factions[State->FactionPerspective]->PendingDecisions)
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

FUDDealListInfo UUDActionAdministrator::GetAllLocalDeals()
{
	FUDDealListInfo info;

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
			dealInfo.DealId = deal.Key;
			dealInfo.Name = deal.Value->Name;
			info.History.Add(dealInfo);
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
		if (State->Deals[dealId]->Participants.Contains(nonmember.Key) || 
			!IsFactionPlayerControlled(nonmember.Key))
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

TArray<FUDDealPointMinimalInfo> UUDActionAdministrator::GetDealPrimaryPointList(int32 dealId)
{
	TArray<FUDDealPointMinimalInfo> primaryPoints = { };
	for (const auto& primaryId : State->Deals[dealId]->PrimaryPoints)
	{
		FUDDealPointMinimalInfo info = FUDDealPointMinimalInfo();
		info.DealId = dealId;
		info.PointId = primaryId;
		primaryPoints.Add(info);
	}
	return primaryPoints;
}

TArray<FUDDealPointMinimalInfo> UUDActionAdministrator::GetDealSecondaryPointList(int32 dealId, int32 pointId)
{
	TArray<FUDDealPointMinimalInfo> secondaryPoints = { };
	for (const auto& pointItem : State->Deals[dealId]->Points[pointId]->Consequencies)
	{
		FUDDealPointMinimalInfo info = FUDDealPointMinimalInfo();
		info.DealId = dealId;
		info.PointId = pointItem;
		secondaryPoints.Add(info);
	}
	return secondaryPoints;
}

TArray<FUDDealPointMinimalInfo> UUDActionAdministrator::GetDealTertiaryPointList(int32 dealId, int32 pointId)
{
	TArray<FUDDealPointMinimalInfo> tertiaryPoints = { };
	for (const auto& pointItem : State->Deals[dealId]->Points[pointId]->Consequencies)
	{
		FUDDealPointMinimalInfo info = FUDDealPointMinimalInfo();
		info.DealId = dealId;
		info.PointId = pointItem;
		tertiaryPoints.Add(info);
	}
	return tertiaryPoints;
}

TArray<FUDDealActionMinimalInfo> UUDActionAdministrator::GetDealActionList(int32 dealId)
{
	TArray<FUDDealActionMinimalInfo> actions = { };

	for (const auto& item : State->Deals[dealId]->DealActionList)
	{
		FUDDealActionMinimalInfo info = FUDDealActionMinimalInfo();
		info.ActionIndex = actions.Num();
		info.DealId = dealId;

		actions.Add(info);
	}

	return actions;
}

// this is tied to contract action at the moment
// if preview is not required it could go away ?
// technically it can be produced from deal data
#include "Core/Simulation/Actions/UDDealActionContractCreate.h"
TArray<FUDActionData> UUDActionAdministrator::GetDealPointsActions(int32 dealId)
{
	TArray<FUDActionData> actions = { };
	actions = UUDDealActionContractCreate::FinalizeActions(State, dealId);
	return actions;
}

FUDParameterListInfo UUDActionAdministrator::GetDealActionParameters(const TSet<int32>& tags)
{
	// TLDR:
	// DealAction and FactionInvoker are not standard parameters, so they are added on top of normal parameters.
	// This way we utilize existing functionality to handle value and text parameters while dealing with additional
	// parameters at the same time.
	// As long as they are not parameters, they are also not a tags...
	FUDParameterListInfo parameters;
	parameters.OrderedType.Empty(0);
	parameters.OrderedData.Empty(0);
	// This determines actions that can belong to the point.
	parameters.OrderedType.Add(EUDParameterType::DealAction);
	parameters.OrderedData.Add(GetActionParameter(tags));
	// This determines invoker parameter.
	parameters.OrderedType.Add(EUDParameterType::FactionInvoker);
	parameters.OrderedData.Add(GetFactionParameter(tags));
	// Always exclude deal ?
	FUDParameterListInfo valueTextParameters = GetActionParameters(tags, UD_INVALID_TAG_ID);
	parameters.OrderedType.Append(valueTextParameters.OrderedType);
	parameters.OrderedData.Append(valueTextParameters.OrderedData);
	return parameters;
}

FUDParameterListInfo UUDActionAdministrator::GetDealActionParametersWithValues(const TSet<int32>& tags, int32 dealAction, FUDActionData data)
{
	FUDParameterListInfo parameters = GetDealActionParameters(tags);

	int32 index = 0;
	for (int32 i = 0; i < parameters.OrderedType.Num(); i++)
	{
		switch (parameters.OrderedType[i])
		{
		case EUDParameterType::DealAction:
			parameters.OrderedData[i].Get<FUDActionParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDActionParameter>().CurrentValue = dealAction;
			break;
		case EUDParameterType::FactionInvoker:
			parameters.OrderedData[i].Get<FUDFactionParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDFactionParameter>().CurrentValue = data.InvokerFactionId;
			break;
		case EUDParameterType::FactionTarget:
			parameters.OrderedData[i].Get<FUDFactionParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDFactionParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Tile:
			parameters.OrderedData[i].Get<FUDTileParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDTileParameter>().CurrentValue = FIntPoint(data.ValueParameters[index++], data.ValueParameters[index++]);
			break;
		case EUDParameterType::Action:
			parameters.OrderedData[i].Get<FUDActionParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDActionParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Resource:
			parameters.OrderedData[i].Get<FUDResourceParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDResourceParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Value:
			parameters.OrderedData[i].Get<FUDValueParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDValueParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Text:
			parameters.OrderedData[i].Get<FUDTextParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDTextParameter>().CurrentValue = data.TextParameter;
		}
	}
	return parameters;
}

FUDParameterListInfo UUDActionAdministrator::GeActionParametersWithValues(const TSet<int32>& tags, FUDActionData data)
{
	FUDParameterListInfo parameters = GetActionParameters(tags, UD_INVALID_TAG_ID);

	int32 index = 0;
	for (int32 i = 0; i < parameters.OrderedType.Num(); i++)
	{
		switch (parameters.OrderedType[i])
		{
		case EUDParameterType::FactionTarget:
			parameters.OrderedData[i].Get<FUDFactionParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDFactionParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Tile:
			parameters.OrderedData[i].Get<FUDTileParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDTileParameter>().CurrentValue = FIntPoint(data.ValueParameters[index++], data.ValueParameters[index++]);
			break;
		case EUDParameterType::Action:
			parameters.OrderedData[i].Get<FUDActionParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDActionParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Resource:
			parameters.OrderedData[i].Get<FUDResourceParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDResourceParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Value:
			parameters.OrderedData[i].Get<FUDValueParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDValueParameter>().CurrentValue = data.ValueParameters[index++];
			break;
		case EUDParameterType::Text:
			parameters.OrderedData[i].Get<FUDTextParameter>().HasCurrentValue = true;
			parameters.OrderedData[i].Get<FUDTextParameter>().CurrentValue = data.TextParameter;
		}
	}
	return parameters;
}

FUDPointInteractionInfo UUDActionAdministrator::GetPointInteraction(int32 dealId, int32 pointId)
{
	FUDPointInteractionInfo interaction;
	interaction.DealId = dealId;
	interaction.PointId = pointId;
	// We start with a discussion point.
	const auto& pointData = State->Deals[dealId]->Points[pointId];
	// Determine action that is used for all parameters.
	interaction.PointActionTypeId = pointData->ActionId;
	const auto& executor = ActionManager->GetAction(pointData->ActionId);
	const auto& executorPresentation = executor->GetPresentation();
	// Once we have action we can move to determining the rest.
	// TODO properly named points...
	interaction.PointTitle = executorPresentation.Name;

	// Extend with dummy parameters, TODO move this to action code!
	TArray<int32> valueParams = pointData->ValueParameters;
	int32 desiredCount = executor->GetParameterCount();
	while (valueParams.Num() < desiredCount)
	{
		valueParams.Add(-1);
	}
	// Fill the action.
	FUDActionData dummyData = FUDActionData(pointData->ActionId, pointData->Invoker, valueParams, pointData->TextParameter);
	interaction.Parameters = GetDealActionParametersWithValues(executorPresentation.Tags, pointData->ActionId, dummyData);
	interaction.PointContent = GetFormattedPresentationString(executorPresentation.DealContentFormat, executorPresentation.Tags, dummyData);
	// TODO find use for type, like seriously it's ignored in all iterations...
	//pointData->Type;

	// TODO parameters should try to load default value as selected before firing any update event!
	return interaction;
}

FUDActionInteractionInfo UUDActionAdministrator::GetActionInteraction(int32 dealId, int32 indexId)
{
	FUDActionInteractionInfo interaction;
	interaction.DealId = dealId;
	interaction.PointIndex = indexId;
	// We start with a discussion action.
	const auto& pointData = State->Deals[dealId]->DealActionList[indexId];
	// Determine action that is used for all parameters.
	interaction.PointActionTypeId = pointData.Action.ActionTypeId;
	const auto& executor = ActionManager->GetAction(pointData.Action.ActionTypeId);
	const auto& executorPresentation = executor->GetPresentation();
	// Once we have action we can move to determining the rest.
	// TODO properly named points...
	interaction.ActionTitle = executorPresentation.Name;
	// Fill the action.
	interaction.Parameters = GeActionParametersWithValues(executorPresentation.Tags, pointData.Action);
	interaction.ActionContent = GetFormattedPresentationString(executorPresentation.DealContentFormat, executorPresentation.Tags, pointData.Action);

	interaction.IsInteractable = pointData.Action.InvokerFactionId == State->FactionPerspective && 
		pointData.SelectedResult == EUDDealActionResult::Unresolved;
	interaction.IsSabotageable = pointData.Action.InvokerFactionId != State->FactionPerspective;

	return interaction;
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

bool UUDActionAdministrator::IsNeutral(int32 factionId)
{
	return UUDGlobalData::GaiaFactionId == factionId;
}

#pragma region AI Checks
TArray<FUDTileMinimalInfo> UUDActionAdministrator::GetNeutralTiles()
{
	TArray<FUDTileMinimalInfo> tiles = {};

	for (const auto& tileData : State->Map->Tiles)
	{
		if (IsNeutral(tileData->OwnerUniqueId))
			continue;
		FUDTileMinimalInfo info;
		info.Name = tileData->Name;
		info.Position = tileData->Position;
		tiles.Add(info);
	}

	return tiles;
}
#pragma endregion