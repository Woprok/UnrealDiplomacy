// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDActionDatabase.h"
#include "Core/Simulation/UDActionInterface.h"

// Default
#include "Core/Simulation/Actions/UDDefaultAction.h"
#include "Core/Simulation/Actions/UDDefaultActionInvalid.h"
// Deal
#include "Core/Simulation/Actions/UDDealAction.h"
#include "Core/Simulation/Actions/UDDealActionContractCreate.h"
#include "Core/Simulation/Actions/UDDealActionContractExecute.h"
#include "Core/Simulation/Actions/UDDealActionContractReputationBonus.h"
#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"
#include "Core/Simulation/Actions/UDDealActionContractPointReject.h"
#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"
#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"
#include "Core/Simulation/Actions/UDDealActionCreate.h"
#include "Core/Simulation/Actions/UDDealActionFinalize.h"
#include "Core/Simulation/Actions/UDDealActionMessageSend.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteConsequence.h"
#include "Core/Simulation/Actions/UDDealActionParticipantKick.h"
#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"
#include "Core/Simulation/Actions/UDDealActionPointAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointIgnore.h"
#include "Core/Simulation/Actions/UDDealActionPointModify.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyAction.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyInvoker.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyValueParameters.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyResetParameters.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTextParameter.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTile.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTileValue.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyType.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyValue.h"
#include "Core/Simulation/Actions/UDDealActionPointRemove.h"
#include "Core/Simulation/Actions/UDDealActionReady.h"
#include "Core/Simulation/Actions/UDDealActionReadyReset.h"
#include "Core/Simulation/Actions/UDDealActionReadyRevert.h"
#include "Core/Simulation/Actions/UDDealActionResultClose.h"
#include "Core/Simulation/Actions/UDDealActionResultDisassemble.h"
#include "Core/Simulation/Actions/UDDealActionResultPass.h"
#include "Core/Simulation/Actions/UDDealActionResultVeto.h"
#include "Core/Simulation/Actions/UDDealActionStateAssemble.h"
#include "Core/Simulation/Actions/UDDealActionStateExtendingDraft.h"
#include "Core/Simulation/Actions/UDDealActionVoteNo.h"
#include "Core/Simulation/Actions/UDDealActionVoteYes.h"
// Decision
#include "Core/Simulation/Actions/UDDecisionAction.h"
#include "Core/Simulation/Actions/UDDecisionActionCreate.h"
#include "Core/Simulation/Actions/UDDecisionActionConfirm.h"
#include "Core/Simulation/Actions/UDDecisionActionDecline.h"
#include "Core/Simulation/Actions/UDDecisionActionConsequenceSelect.h"
// Gaia
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionResourceDeficit.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionResourceYield.h"
#include "Core/Simulation/Actions/UDGaiaActionTileResourceYield.h"
#include "Core/Simulation/Actions/UDGaiaActionTileUpkeep.h"
#include "Core/Simulation/Actions/UDGaiaActionFactionStratagemActivated.h"
#include "Core/Simulation/Actions/UDGaiaActionTileStratagemActivated.h"
#include "Core/Simulation/Actions/UDGaiaActionStratagemActivatedReset.h"
#include "Core/Simulation/Actions/UDGaiaActionStratagemResourceCost.h"
#include "Core/Simulation/Actions/UDGaiaActionThroneReceive.h"
#include "Core/Simulation/Actions/UDGaiaActionDecisionReset.h"
// Game
#include "Core/Simulation/Actions/UDGameAction.h"
#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDGameActionPermitTileExploit.h"
#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"
#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/Simulation/Actions/UDGameActionThroneContest.h"
#include "Core/Simulation/Actions/UDGameActionThroneLiberate.h"
#include "Core/Simulation/Actions/UDGameActionTileExploit.h"
#include "Core/Simulation/Actions/UDGameActionTileTake.h"
#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "Core/Simulation/Actions/UDGameActionThroneSupport.h"
#include "Core/Simulation/Actions/UDGameActionThroneSupportCancel.h"
#include "Core/Simulation/Actions/UDGameActionMilitarySupport.h"
#include "Core/Simulation/Actions/UDGameActionMilitarySupportCancel.h"
#include "Core/Simulation/Actions/UDGameActionNone.h"
#include "Core/Simulation/Actions/UDGameActionRumour.h"
#include "Core/Simulation/Actions/UDGameActionBurglary.h"
#include "Core/Simulation/Actions/UDGameActionTileRaid.h"
#include "Core/Simulation/Actions/UDGameActionStratagemShare.h"
#include "Core/Simulation/Actions/UDGameActionTileBuildFarm.h"
#include "Core/Simulation/Actions/UDGameActionTileBuildFortress.h"
#include "Core/Simulation/Actions/UDGameActionTileBuildManufactury.h"
#include "Core/Simulation/Actions/UDGameActionTileBuildPalace.h"
#include "Core/Simulation/Actions/UDGameActionTileBuildQuarry.h"
#include "Core/Simulation/Actions/UDGameActionTileBuildTradeGuild.h"
// Setting
#include "Core/Simulation/Actions/UDSettingAction.h"
#include "Core/Simulation/Actions/UDSettingActionAICountChange.h"
#include "Core/Simulation/Actions/UDSettingActionFactionRename.h"
#include "Core/Simulation/Actions/UDSettingActionMapSeedChange.h"
#include "Core/Simulation/Actions/UDSettingActionMapWidthChange.h"
#include "Core/Simulation/Actions/UDSettingActionMapHeightChange.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemPointsChange.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemOptionSelect.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemOptionDeselect.h"
// System
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "Core/Simulation/Actions/UDSystemActionGameEnd.h"
#include "Core/Simulation/Actions/UDSystemActionGameStart.h"
#include "Core/Simulation/Actions/UDSystemActionLog.h"
#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"
#include "Core/Simulation/Actions/UDSystemActionPlayerRemove.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "Core/Simulation/Actions/UDSystemActionRegentChange.h"
#include "Core/Simulation/Actions/UDSystemActionWorldCreate.h"
#include "Core/Simulation/Actions/UDSystemActionWorldSpawnFactions.h"
#include "Core/Simulation/Actions/UDSystemActionFactionTakeover.h"
#include "Core/Simulation/Actions/UDSystemActionTurnFinish.h"
#include "Core/Simulation/Actions/UDSystemActionIntermezzoStart.h"
#include "Core/Simulation/Actions/UDSystemActionIntermezzoEnd.h"

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetDefaultActions(UObject* parent)
{
	return {
		NewObject<UUDDefaultAction>(parent),
		NewObject<UUDDefaultActionInvalid>(parent),
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetSystemActions(UObject* parent)
{
	return {
		//NewObject<UUDSystemAction>(parent),
		NewObject<UUDSystemActionGameEnd>(parent),
		NewObject<UUDSystemActionGameStart>(parent),
		NewObject<UUDSystemActionLog>(parent),
		NewObject<UUDSystemActionPlayerAdd>(parent),
		//NewObject<UUDSystemActionPlayerRemove>(parent),
		NewObject<UUDSystemActionTurnEnd>(parent),
		NewObject<UUDSystemActionRegentChange>(parent),
		NewObject<UUDSystemActionWorldCreate>(parent),
		NewObject<UUDSystemActionWorldSpawnFactions>(parent),
		NewObject<UUDSystemActionFactionTakeover>(parent),
		NewObject<UUDSystemActionTurnFinish>(parent),
		NewObject<UUDSystemActionIntermezzoStart>(parent),
		NewObject<UUDSystemActionIntermezzoEnd>(parent),
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetDecisionActions(UObject* parent)
{
	return {
		//NewObject<UDDecisionAction>(parent),
		NewObject<UUDDecisionActionCreate>(parent),
		NewObject<UUDDecisionActionConfirm>(parent),
		NewObject<UUDDecisionActionDecline>(parent),
		NewObject<UUDDecisionActionConsequenceSelect>(parent),
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetGaiaActions(UObject* parent)
{
	return {
		//NewObject<UUDGaiaAction>(parent),
		NewObject<UUDGaiaActionFactionResourceDeficit>(parent),
		NewObject<UUDGaiaActionFactionResourceYield>(parent),
		NewObject<UUDGaiaActionTileResourceYield>(parent),
		NewObject<UUDGaiaActionTileUpkeep>(parent),
		NewObject<UUDGaiaActionFactionStratagemActivated>(parent),
		NewObject<UUDGaiaActionTileStratagemActivated>(parent),
		NewObject<UUDGaiaActionStratagemActivatedReset>(parent),
		NewObject<UUDGaiaActionStratagemResourceCost>(parent),
		NewObject<UUDGaiaActionThroneReceive>(parent),
		NewObject<UUDGaiaActionDecisionReset>(parent),
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetGameActions(UObject* parent)
{
	return {
		//NewObject<UUDGameAction>(parent),
		NewObject<UUDGameActionGift>(parent),
		NewObject<UUDGameActionPermitTileExploit>(parent),
		NewObject<UUDGameActionThroneAbdicate>(parent),
		NewObject<UUDGameActionThroneUsurp>(parent),
		NewObject<UUDGameActionThroneContest>(parent),
		NewObject<UUDGameActionThroneLiberate>(parent),
		NewObject<UUDGameActionTileExploit>(parent),
		NewObject<UUDGameActionTileTake>(parent),
		NewObject<UUDGameActionTileTransfer>(parent),
		NewObject<UUDGameActionThroneSupport>(parent),
		NewObject<UUDGameActionThroneSupportCancel>(parent),
		NewObject<UUDGameActionMilitarySupport>(parent),
		NewObject<UUDGameActionMilitarySupportCancel>(parent),
		NewObject<UUDGameActionNone>(parent),
		NewObject<UUDGameActionRumour>(parent),
		NewObject<UUDGameActionBurglary>(parent),
		NewObject<UUDGameActionTileRaid>(parent),
		NewObject<UUDGameActionStratagemShare>(parent),
		NewObject<UUDGameActionTileBuildFarm>(parent),
		NewObject<UUDGameActionTileBuildFortress>(parent),
		NewObject<UUDGameActionTileBuildManufactury>(parent),
		NewObject<UUDGameActionTileBuildPalace>(parent),
		NewObject<UUDGameActionTileBuildQuarry>(parent),
		NewObject<UUDGameActionTileBuildTradeGuild>(parent),
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetDealActions(UObject* parent)
{
	return {
		//NewObject<UUDDealAction>(parent),
		NewObject<UUDDealActionContractCreate>(parent),
		NewObject<UUDDealActionContractExecute>(parent),
		NewObject<UUDDealActionContractReputationBonus>(parent),
		NewObject<UUDDealActionContractPointAccept>(parent),
		NewObject<UUDDealActionContractPointReject>(parent),
		NewObject<UUDDealActionContractPointSabotage>(parent),
		NewObject<UUDDealActionContractPointTamper>(parent),
		NewObject<UUDDealActionCreate>(parent),
		//NewObject<UUDDealActionFinalize>(parent),
		NewObject<UUDDealActionMessageSend>(parent),
		NewObject<UUDDealActionParticipantInvite>(parent),
		NewObject<UUDDealActionParticipantInviteConsequence>(parent),
		NewObject<UUDDealActionParticipantKick>(parent),
		NewObject<UUDDealActionParticipantLeave>(parent),
		NewObject<UUDDealActionPointAdd>(parent),
		NewObject<UUDDealActionPointChildAdd>(parent),
		NewObject<UUDDealActionPointIgnore>(parent),
		//NewObject<UUDDealActionPointModify>(parent),
		NewObject<UUDDealActionPointModifyAction>(parent),
		NewObject<UUDDealActionPointModifyType>(parent),
		NewObject<UUDDealActionPointModifyResetParameters>(parent),
		NewObject<UUDDealActionPointModifyInvoker>(parent),
		NewObject<UUDDealActionPointModifyValueParameters>(parent),
		NewObject<UUDDealActionPointModifyTextParameter>(parent),
		//NewObject<UUDDealActionPointModifyTile>(parent),
		//NewObject<UUDDealActionPointModifyTileValue>(parent),
		//NewObject<UUDDealActionPointModifyValue>(parent),
		//NewObject<UUDDealActionPointRemove>(parent),
		NewObject<UUDDealActionReady>(parent),
		//NewObject<UUDDealActionReadyReset>(parent),
		NewObject<UUDDealActionReadyRevert>(parent),
		NewObject<UUDDealActionResultClose>(parent),
		NewObject<UUDDealActionResultDisassemble>(parent),
		NewObject<UUDDealActionResultPass>(parent),
		NewObject<UUDDealActionResultVeto>(parent),
		NewObject<UUDDealActionStateAssemble>(parent),
		NewObject<UUDDealActionStateExtendingDraft>(parent),
		NewObject<UUDDealActionVoteNo>(parent),
		NewObject<UUDDealActionVoteYes>(parent),
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetSettingActions(UObject* parent)
{
	return {
		//NewObject<UUDSettingAction>(parent),
		NewObject<UUDSettingActionAICountChange>(parent),
		NewObject<UUDSettingActionFactionRename>(parent),
		NewObject<UUDSettingActionMapSeedChange>(parent),
		NewObject<UUDSettingActionMapWidthChange>(parent),
		NewObject<UUDSettingActionMapHeightChange>(parent),
		NewObject<UUDSettingActionStratagemPointsChange>(parent),
		NewObject<UUDSettingActionStratagemOptionSelect>(parent),
		NewObject<UUDSettingActionStratagemOptionDeselect>(parent),
	};
}