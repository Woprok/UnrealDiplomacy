// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneContest.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Resources/UDGameResourceManpower.h"
#include "Core/Simulation/Modifiers/UDFactionModifierMilitarySupport.h"

bool UUDGameActionThroneContest::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isThroneUsurped = world->ImperialThrone.Ruler != UUDGlobalData::GaiaFactionId;
	bool isNotThroneUsurper = world->ImperialThrone.Ruler != action.InvokerFactionId;
	//bool hasStratagem = world->Factions[action.InvokerFactionId]->StratagemOptions.Contains(ActionTypeId);
	return IUDActionInterface::CanExecute(action, world) && isThroneUsurped && isNotThroneUsurper;//&& hasStratagem;
}

void UUDGameActionThroneContest::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Execute(action, world);

	int32 contesterArmyStrength = GetTotalMilitarySupport(world, action.InvokerFactionId);
	int32 usurperArmyStrength = GetTotalMilitarySupport(world, world->ImperialThrone.Ruler);

	// Fight for the throne.
	if (contesterArmyStrength > usurperArmyStrength * UsurperManpowerDefensiveBonus)
	{
		float contesterWon = ContesterSideManpowerCasualties + WinnerCasualties;
		float usurperLost = UsurperSideManpowerCasualties + LoserCasualties;
		ApplySideCasualties(world, action.InvokerFactionId, contesterWon);
		ApplySideCasualties(world, world->ImperialThrone.Ruler, usurperLost);
		// Contester claims the throne finally.
		world->ImperialThrone.Ruler = action.InvokerFactionId;
	}
	else
	{
		float usurperWon = UsurperSideManpowerCasualties + WinnerCasualties;
		float contesterLost = ContesterSideManpowerCasualties + LoserCasualties;
		ApplySideCasualties(world, action.InvokerFactionId, contesterLost);
		ApplySideCasualties(world, world->ImperialThrone.Ruler, usurperWon);
		// Throne is kept by usurper.
	}
}

void UUDGameActionThroneContest::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Rollback usurper throne.
	FUDGameDataTarget oldData(action.BackupValueParameters);
	world->ImperialThrone.Ruler = oldData.TargetId;
	// TODO add revert for manpower loses... 
}

#define LOCTEXT_NAMESPACE "ThroneContest"
FUDActionPresentation UUDGameActionThroneContest::GetPresentation() const
{
	if (GetId() != UUDGameActionThroneContest::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneContest", "Throne Contest")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_2,
		}
	);
	presentation.DealContentFormat = FText(LOCTEXT("ThroneContest",
		"Faction [{INVOKER}] will attempt to contest the throne from usurper hands with his armny."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE

void UUDGameActionThroneContest::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old param is backuped for future revert use.
	// stored as FUDGameDataTarget, due to being single param in the list
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(world->ImperialThrone.Ruler);
}

void UUDGameActionThroneContest::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}

void UUDGameActionThroneContest::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

int32 UUDGameActionThroneContest::GetTotalMilitarySupport(const TObjectPtr<UUDWorldState>& state, int32 factionId)
{
	int32 baseSupport = ResourceManager->GetCurrent(state->Factions[factionId], UUDGameResourceManpower::ResourceId);
	int32 additionalSupport = 0;

	for (const auto& modifier : ModifierManager->GetAllFactionModifiers(
		state->Factions[factionId],
		UUDFactionModifierMilitarySupport::ModifierTypeId))
	{
		additionalSupport += ResourceManager->GetCurrent(state->Factions[modifier.InvokerId], UUDGameResourceManpower::ResourceId);
	}

	return baseSupport + additionalSupport;
}

void UUDGameActionThroneContest::ApplySideCasualties(const TObjectPtr<UUDWorldState>& state, int32 factionId, float losesPercantage)
{
	// Leader of the side gets largest casualties.
	ResourceManager->SubstractPercentage(state->Factions[factionId], UUDGameResourceManpower::ResourceId, losesPercantage + LeaderCasualties);

	// Supprters of the side suffer less, but still enough to be punished for fight
	for (const auto& modifier : ModifierManager->GetAllFactionModifiers(
		state->Factions[factionId],
		UUDFactionModifierMilitarySupport::ModifierTypeId))
	{
		ResourceManager->SubstractPercentage(state->Factions[modifier.InvokerId], UUDGameResourceManpower::ResourceId, losesPercantage);
	}
}