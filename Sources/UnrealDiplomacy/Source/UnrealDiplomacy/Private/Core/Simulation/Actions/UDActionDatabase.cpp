// Copyright Miroslav Valach

#include "Core/Simulation/Actions/UDActionDatabase.h"
#include "Core/Simulation/UDActionInterface.h"

// Default
#include "Core/Simulation/Actions/UDDefaultActionInvalid.h"
// Deal
#include "Core/Simulation/Actions/UDDealAction.h"
#include "Core/Simulation/Actions/UDDealActionContractCreate.h"
#include "Core/Simulation/Actions/UDDealActionContractExecute.h"
#include "Core/Simulation/Actions/UDDealActionContractPointAccept.h"
#include "Core/Simulation/Actions/UDDealActionContractPointReject.h"
#include "Core/Simulation/Actions/UDDealActionContractPointSabotage.h"
#include "Core/Simulation/Actions/UDDealActionContractPointTamper.h"
#include "Core/Simulation/Actions/UDDealActionCreate.h"
#include "Core/Simulation/Actions/UDDealActionFinalize.h"
#include "Core/Simulation/Actions/UDDealActionMessageSend.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteAccept.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInviteReject.h"
#include "Core/Simulation/Actions/UDDealActionParticipantKick.h"
#include "Core/Simulation/Actions/UDDealActionParticipantLeave.h"
#include "Core/Simulation/Actions/UDDealActionPointAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointChildAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointIgnore.h"
#include "Core/Simulation/Actions/UDDealActionPointModify.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyAction.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyInvokerRemove.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyResetParameters.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTargetAdd.h"
#include "Core/Simulation/Actions/UDDealActionPointModifyTargetRemove.h"
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
// Gaia
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "Core/Simulation/Actions/UDGaiaActionResourcesAllAdd.h"
// Game
#include "Core/Simulation/Actions/UDGameAction.h"
#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "Core/Simulation/Actions/UDGameActionGiftAccept.h"
#include "Core/Simulation/Actions/UDGameActionGiftIrrevocable.h"
#include "Core/Simulation/Actions/UDGameActionGiftReject.h"
#include "Core/Simulation/Actions/UDGameActionPermitTileExploit.h"
#include "Core/Simulation/Actions/UDGameActionThroneAbdicate.h"
#include "Core/Simulation/Actions/UDGameActionThroneReceive.h"
#include "Core/Simulation/Actions/UDGameActionThroneUsurp.h"
#include "Core/Simulation/Actions/UDGameActionTileExploit.h"
#include "Core/Simulation/Actions/UDGameActionTileTake.h"
#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "Core/Simulation/Actions/UDGameActionTileTransferAccept.h"
#include "Core/Simulation/Actions/UDGameActionTileTransferReject.h"
// System
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "Core/Simulation/Actions/UDSystemActionGameEnd.h"
#include "Core/Simulation/Actions/UDSystemActionGameStart.h"
#include "Core/Simulation/Actions/UDSystemActionLog.h"
#include "Core/Simulation/Actions/UDSystemActionPlayerAdd.h"
#include "Core/Simulation/Actions/UDSystemActionPlayerRemove.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "Core/Simulation/Actions/UDSystemActionTurnForceEnd.h"
#include "Core/Simulation/Actions/UDSystemActionWorldCreate.h"

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetDefaultActions(UObject* parent)
{
	return {
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
		NewObject<UUDSystemActionTurnForceEnd>(parent),
		NewObject<UUDSystemActionWorldCreate>(parent)
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetGaiaActions(UObject* parent)
{
	return {
		//NewObject<UUDGaiaAction>(parent),
		NewObject<UUDGaiaActionResourcesAllAdd>(parent)
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetGameActions(UObject* parent)
{
	return {
		//NewObject<UUDGameAction>(parent),
		NewObject<UUDGameActionGift>(parent),
		NewObject<UUDGameActionGiftAccept>(parent),
		NewObject<UUDGameActionGiftIrrevocable>(parent),
		NewObject<UUDGameActionGiftReject>(parent),
		NewObject<UUDGameActionPermitTileExploit>(parent),
		NewObject<UUDGameActionThroneAbdicate>(parent),
		NewObject<UUDGameActionThroneReceive>(parent),
		NewObject<UUDGameActionThroneUsurp>(parent),
		NewObject<UUDGameActionTileExploit>(parent),
		NewObject<UUDGameActionTileTake>(parent),
		NewObject<UUDGameActionTileTransfer>(parent),
		NewObject<UUDGameActionTileTransferAccept>(parent),
		NewObject<UUDGameActionTileTransferReject>(parent)
	};
}

TArray<TScriptInterface<IUDActionInterface>> UUDActionDatabase::GetDealActions(UObject* parent)
{
	return {
		//NewObject<UUDDealAction>(parent),
		NewObject<UUDDealActionContractCreate>(parent),
		NewObject<UUDDealActionContractExecute>(parent),
		NewObject<UUDDealActionContractPointAccept>(parent),
		NewObject<UUDDealActionContractPointReject>(parent),
		NewObject<UUDDealActionContractPointSabotage>(parent),
		NewObject<UUDDealActionContractPointTamper>(parent),
		NewObject<UUDDealActionCreate>(parent),
		//NewObject<UUDDealActionFinalize>(parent),
		NewObject<UUDDealActionMessageSend>(parent),
		NewObject<UUDDealActionParticipantInvite>(parent),
		NewObject<UUDDealActionParticipantInviteAccept>(parent),
		NewObject<UUDDealActionParticipantInviteReject>(parent),
		NewObject<UUDDealActionParticipantKick>(parent),
		NewObject<UUDDealActionParticipantLeave>(parent),
		NewObject<UUDDealActionPointAdd>(parent),
		NewObject<UUDDealActionPointChildAdd>(parent),
		NewObject<UUDDealActionPointIgnore>(parent),
		//NewObject<UUDDealActionPointModify>(parent),
		NewObject<UUDDealActionPointModifyAction>(parent),
		NewObject<UUDDealActionPointModifyInvokerAdd>(parent),
		NewObject<UUDDealActionPointModifyInvokerRemove>(parent),
		NewObject<UUDDealActionPointModifyResetParameters>(parent),
		NewObject<UUDDealActionPointModifyTargetAdd>(parent),
		NewObject<UUDDealActionPointModifyTargetRemove>(parent),
		NewObject<UUDDealActionPointModifyTile>(parent),
		NewObject<UUDDealActionPointModifyTileValue>(parent),
		NewObject<UUDDealActionPointModifyType>(parent),
		NewObject<UUDDealActionPointModifyValue>(parent),
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
		NewObject<UUDDealActionVoteYes>(parent)
	};
}