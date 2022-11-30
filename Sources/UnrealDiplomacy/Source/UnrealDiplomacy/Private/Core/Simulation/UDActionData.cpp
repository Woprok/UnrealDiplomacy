// Copyright Miroslav Valach


#include "Core/Simulation/UDActionData.h"

FUDActionData::FUDActionData()
{

}

FUDActionData::FUDActionData(const FUDActionData& existingAction, int32 ActionTypeId) :
	ActionTypeId(ActionTypeId),
	InvokerPlayerId(existingAction.InvokerPlayerId),
	TargetPlayerId(existingAction.TargetPlayerId),
	ValueParameter(existingAction.ValueParameter),
	TileParameter(existingAction.TileParameter),
	UniqueId(existingAction.UniqueId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId) : ActionTypeId(actionTypeId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId) : ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId)
	: ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId), TargetPlayerId(targetPlayerId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId, int32 valueParameter)
	: ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId), TargetPlayerId(targetPlayerId), ValueParameter(valueParameter)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 valueParameter, FIntPoint tileParameter)
	: ActionTypeId(actionTypeId), ValueParameter(valueParameter), TileParameter(tileParameter)
{

}