// Copyright Miroslav Valach


#include "Core/Simulation/UDActionData.h"

FUDActionData::FUDActionData()
{

} 

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId) : ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId)
{

}