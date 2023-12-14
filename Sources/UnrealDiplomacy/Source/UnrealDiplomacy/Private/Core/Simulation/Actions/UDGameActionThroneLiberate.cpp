// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDGameActionThroneLiberate.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDResourceManager.h"
#include "Core/Simulation/UDModifierManager.h"
#include "Core/Simulation/Resources/UDGameResourceManpower.h"
#include "Core/Simulation/Resources/UDGameResourceReputation.h"
#include "Core/Simulation/Modifiers/UDFactionModifierMilitarySupport.h"

bool UUDGameActionThroneLiberate::CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const
{
	bool isThroneUsurped = world->ImperialThrone.Ruler != UUDGlobalData::GaiaFactionId;
	bool isNotThroneUsurper = world->ImperialThrone.Ruler != action.InvokerFactionId;

	return IUDActionInterface::CanExecute(action, world) && isThroneUsurped && isNotThroneUsurper;
}

void UUDGameActionThroneLiberate::Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
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
		// Throne is finally liberated.
		int32 leftover = ResourceManager->GetCurrent(world->Factions[world->ImperialThrone.Ruler], UUDGameResourceReputation::ResourceId);
		if (leftover > 0)
		{
			int32 leftoverMult = LiberatorReputationWin + leftover * UsurperReputationTakePerc;
			ResourceManager->Substract(world->Factions[world->ImperialThrone.Ruler], UUDGameResourceReputation::ResourceId, leftover);
			ResourceManager->Add(world->Factions[action.InvokerFactionId], UUDGameResourceReputation::ResourceId, leftoverMult);
		}
		else
		{
			ResourceManager->Add(world->Factions[action.InvokerFactionId], UUDGameResourceReputation::ResourceId, LiberatorReputationWin);
		}

		// Finally free the throne.
		world->ImperialThrone.Ruler = UUDGlobalData::GaiaFactionId;
	}
	else
	{
		float usurperWon = UsurperSideManpowerCasualties + WinnerCasualties;
		float contesterLost = ContesterSideManpowerCasualties + LoserCasualties;
		ApplySideCasualties(world, action.InvokerFactionId, contesterLost);
		ApplySideCasualties(world, world->ImperialThrone.Ruler, usurperWon);
		// Throne is kept by usurper and he gains some legitimacy, kind of...
		ResourceManager->Add(world->Factions[world->ImperialThrone.Ruler], UUDGameResourceReputation::ResourceId, UsurperReputationWin);
		ResourceManager->Substract(world->Factions[action.InvokerFactionId], UUDGameResourceReputation::ResourceId, UsurperReputationWin);
	}
}

void UUDGameActionThroneLiberate::Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	IUDActionInterface::Revert(action, world);
	// Rollback usurper throne.
	FUDGameDataTarget oldData(action.BackupValueParameters);
	world->ImperialThrone.Ruler = oldData.TargetId;
	// TODO add revert for manpower loses... 
}

#define LOCTEXT_NAMESPACE "ThroneLiberate"
FUDActionPresentation UUDGameActionThroneLiberate::GetPresentation() const
{
	if (GetId() != UUDGameActionThroneLiberate::ActionTypeId)
		return Super::GetPresentation();
	FUDActionPresentation presentation = FUDActionPresentation();
	presentation.ActionId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneLiberate", "Throne Liberate")).ToString();
	presentation.Tags.Append(
		{
			UD_ACTION_TAG_VALID,
			UD_ACTION_TAG_STRATAGEM,
			UD_ACTION_TAG_STRATAGEM_COST_1,
		}
	);
	presentation.DealContentFormat = FText(LOCTEXT("ThroneLiberate",
		"Faction [{INVOKER}] will attempt to liberate the throne from usurper hands with his armny."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE

void UUDGameActionThroneLiberate::Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world)
{
	// Old param is backuped for future revert use.
	// stored as FUDGameDataTarget, due to being single param in the list
	action.BackupValueParameters.Empty(0);
	action.BackupValueParameters.Add(world->ImperialThrone.Ruler);
}

void UUDGameActionThroneLiberate::SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager)
{
	ResourceManager = resourceManager;
}

void UUDGameActionThroneLiberate::SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager)
{
	ModifierManager = modifierManager;
}

int32 UUDGameActionThroneLiberate::GetTotalMilitarySupport(const TObjectPtr<UUDWorldState>& state, int32 factionId)
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

void UUDGameActionThroneLiberate::ApplySideCasualties(const TObjectPtr<UUDWorldState>& state, int32 factionId, float losesPercantage)
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