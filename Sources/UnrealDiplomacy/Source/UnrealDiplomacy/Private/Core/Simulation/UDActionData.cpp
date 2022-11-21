// Copyright Miroslav Valach


#include "Core/Simulation/UDActionData.h"

FUDActionData::FUDActionData()
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