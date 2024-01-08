// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishAIController.h"
#include "Core/Simulation/UDWorldState.h"
#include "Core/Simulation/UDActionData.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDGameActionTileTake.h"
#include "Core/Simulation/Actions/UDGameActionMilitarySupport.h"
#include "Core/Simulation/Actions/UDGameActionMilitarySupportCancel.h"
#include "Core/Simulation/Actions/UDSystemActionTurnFinish.h"
#include "Core/Simulation/Actions/UDGameActionThroneLiberate.h"
#include "Core/Simulation/Actions/UDSettingActionFactionRename.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemOptionSelect.h"

#include "Core/Simulation/Resources/UDGameResourceReputation.h"
#include "Core/Simulation/Resources/UDGameResourceGold.h"
#include "Core/Simulation/Resources/UDGameResourceFood.h"
#include "Core/Simulation/Resources/UDGameResourceMaterials.h"
#include "Core/Simulation/Resources/UDGameResourceManpower.h"
#include "Core/Simulation/Resources/UDGameResourceLuxuries.h"

#include "Core/Simulation/Actions/UDDealActionEndStateDefine.h"
#include "Core/Simulation/Actions/UDDealActionEndStateVote.h"
#include "Core/Simulation/Actions/UDDealActionVoteNo.h"
#include "Core/Simulation/Actions/UDDealActionVoteYes.h"
#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"
#include "Core/Simulation/Actions/UDDealActionContractPointReject.h"

// This AI is very naive and basically accepts everything it can.
// Then it provides boon to anyone who gave it gift, but she has short memory so be last.
// She also hates usurpers and will attack them.

void AUDSkirmishAIController::ProcessPreGamePlay()
{
	if (SetSettings)
		return;
	SetSettings = true;
	MakeAction(UUDSettingActionStratagemOptionSelect::ActionTypeId, { UUDGameActionGift::ActionTypeId });
	MakeAction(UUDSettingActionStratagemOptionSelect::ActionTypeId, { UUDGameActionTileTake::ActionTypeId });
	MakeAction(UUDSettingActionStratagemOptionSelect::ActionTypeId, { UUDGameActionMilitarySupport::ActionTypeId });
	MakeAction(UUDSettingActionStratagemOptionSelect::ActionTypeId, { UUDGameActionMilitarySupportCancel::ActionTypeId });
	MakeAction(UUDSettingActionStratagemOptionSelect::ActionTypeId, { UUDGameActionThroneLiberate::ActionTypeId });
	int32 index = ControlledFactionId % Names.Num();
	MakeAction(UUDSettingActionFactionRename::ActionTypeId, Names[index]);
}

void AUDSkirmishAIController::ProcessOutTurnPlay()
{
	// Resolve requests
	ResolveRequests();
	// Do necessary to accept/reject deal.
	ResolveDeals();
}

int32 AUDSkirmishAIController::GetFirst(const TSet<int32>& iteratedSet)
{
	for (const auto& item : iteratedSet)
	{
		return item;
	}
	return -1;
}

int32 AUDSkirmishAIController::GetFirstIntersecting(const TSet<int32>& iteratedSet, const TSet<int32>& checkedSet)
{
	for (const auto& item : iteratedSet)
	{
		if (checkedSet.Contains(item))
			return item;
	}
	return -1;
}

void AUDSkirmishAIController::ProcessInTurnPlay()
{
	// Resolve requests
	ResolveRequests();
	// Do necessary to accept/reject deal.
	ResolveDeals();

	const auto res = GetAdministrator()->GetLocalFactionResourceList();
	const auto throne = GetAdministrator()->GetThroneInfo();
	TMap<int32, FUDResourceInfo> resources = { };
	for (const auto& t_res : res)
	{
		resources.Add(t_res.Id, t_res);
	}

	// Remove bad supports!
	if (Supports.Num() > 0)
	{
		int32 badActorId = GetFirstIntersecting(Supports, EnemyFactions);

		// Give priority to usurpers in support cancel
		if (throne.State == EUDImperialThroneState::Usurper && Supports.Contains(throne.UsurperId))
			badActorId = throne.UsurperId;

		// Remove support from bad actor
		if (badActorId != -1)
		{
			TArray<int32> endMilitarySupport = {
				badActorId
			};
			Supports.Remove(badActorId);
			MakeAction(UUDGameActionMilitarySupportCancel::ActionTypeId, endMilitarySupport);
		}
	}

	// She is very nice and will definitely no 
	if (throne.State == EUDImperialThroneState::Usurper)
	{
		FavoriteFactions.Remove(throne.UsurperId);
		EnemyFactions.Add(throne.UsurperId);
		MakeAction(UUDGameActionThroneLiberate::ActionTypeId);
	}

	// Give good guys support!
	if (FavoriteFactions.Num() > 0)
	{
		int32 firstId = GetFirst(FavoriteFactions);
		// Try sharing the wealth...
		if (resources[UUDGameResourceGold::ResourceId].Amount >= 100)
		{
			TArray<int32> smallGoldGift = { 
				firstId,
				UUDGameResourceGold::ResourceId, resources[UUDGameResourceGold::ResourceId].Amount / 5
			};
			MakeAction(UUDGameActionGift::ActionTypeId, smallGoldGift);
		}
		// Otherwise give army help
		else
		{
			TArray<int32> militarySupport = {
				firstId
			};
			Supports.Add(firstId);
			MakeAction(UUDGameActionMilitarySupport::ActionTypeId, militarySupport);
		}
		FavoriteFactions.Remove(firstId);
	}

	// Try take some land...
	const auto& tiles = GetAdministrator()->GetNeutralTiles();
	if (tiles.Num() > 0)
	{
		MakeAction(UUDGameActionTileTake::ActionTypeId, { tiles[0].Position.X, tiles[0].Position.Y });
	}

	// Finish this by executing finish turn action, thus giving up primary control.
	MakeAction(UUDSystemActionTurnFinish::ActionTypeId);
}

void AUDSkirmishAIController::ResolveRequests()
{
	for (const auto& request : GetAdministrator()->GetAllLocalRequests().Messages)
	{
		if (request.IsDemand)
		{
			// Naughty are bad, we don't work with them again!
			int32 sourceFactionId = request.CreatorId;
			FavoriteFactions.Remove(sourceFactionId);
			EnemyFactions.Add(sourceFactionId);
			MakeDeclineAction(request.DecisionId);
			continue;
		}

		if (request.Content.ActionTypeId == UUDGameActionGift::ActionTypeId && !request.HasChoices)
		{
			// They want gift, or we get gift ? who cares, we are happy!
			int32 sourceFactionId = request.CreatorId;
			if (!EnemyFactions.Contains(sourceFactionId))
				FavoriteFactions.Add(sourceFactionId);
		}

		MakeConfirmAction(request.DecisionId);
	}
}

void AUDSkirmishAIController::ResolveDeals()
{
	bool isFriendlyDeal = true;
	// AI will generally vote yes and accept all points.
	// AI will vote no and reject all points if it considers any player enemy.

	for (const auto& deal : GetAdministrator()->GetActiveParticipatingDealList())
	{
		// Resolve each deal, by looking directly into the state
		// TODO this needs to be exchanged for mdoel functions.
		// Looking up the state was added only as temporary solution.
		const auto participants = GetAdministrator()->GetDealParticipantList(deal.DealId);
		bool hasEnemy = participants.ContainsByPredicate(
			[this](const FUDDealFactionInfo& faction) {
				return EnemyFactions.Contains(faction.FactionId);
			}
		);
		// friendly is only if it has no enemy.
		isFriendlyDeal = !hasEnemy;

		const auto state = GetAdministrator()->GetOverseeingState()->Deals[deal.DealId]->DealSimulationState;
		const auto dealInfo = GetAdministrator()->GetDealInfo(deal.DealId);
		// handle vote phase
		if (state == EUDDealSimulationState::FinalVote)
		{
			if (isFriendlyDeal && !dealInfo.LocalVote)
			{
				MakeAction(UUDDealActionVoteYes::ActionTypeId, { deal.DealId });
			}
			else if (!isFriendlyDeal && dealInfo.LocalVote)
			{
				MakeAction(UUDDealActionVoteNo::ActionTypeId, { deal.DealId });
			}
		}
		else if (state == EUDDealSimulationState::ResolutionOfPoints)
		{
			// Always accepts as the no option is impossible to trigger for AI as it accepts immediately after phase change.
			// Handles only unresolved request for this AI. Never attempts to sabotage or change them.
			for (const auto& action : GetAdministrator()->GetDealLocalUnresolvedActionList(deal.DealId))
			{
				MakeAction(UUDDealActionContractPointAccept::ActionTypeId, { deal.DealId, action.ActionIndex });
			}
		}
	}
}