// Copyright Miroslav Valach

#include "Core/Simulation/UDActionData.h"

FUDActionData::FUDActionData()
{

}

FUDActionData::FUDActionData(const FUDActionData& actionData) 
	: 
	ActionTypeId(actionData.ActionTypeId),
	UniqueId(actionData.UniqueId),
	ParentUniqueId(actionData.ParentUniqueId),
	InvokerPlayerId(actionData.InvokerPlayerId),
	TargetPlayerId(actionData.TargetPlayerId),
	ValueParameter(actionData.ValueParameter),
	TileParameter(actionData.TileParameter)
{

}

FUDActionData FUDActionData::CreateDataCopy(const FUDActionData& existingAction)
{
	FUDActionData actionData = FUDActionData();
	actionData.InvokerPlayerId = existingAction.InvokerPlayerId;
	actionData.TargetPlayerId = existingAction.TargetPlayerId;
	actionData.ValueParameter = existingAction.ValueParameter;
	actionData.TileParameter = existingAction.TileParameter;
	return actionData;
}

FUDActionData FUDActionData::CreateChild(const FUDActionData& parentAction, int32 ActionTypeId)
{
	FUDActionData child = FUDActionData::CreateDataCopy(parentAction);
	child.ActionTypeId = ActionTypeId;

	// child inherits UniqueId as ParentUniqueId
	child.ParentUniqueId = parentAction.UniqueId;
	// child.UniqueId remains default value 0, so it can be assigned by WorldSimulation

	return child;
}

FUDActionData FUDActionData::CreateParent(const FUDActionData& childAction, int32 ActionTypeId)
{
	FUDActionData parent = FUDActionData::CreateDataCopy(childAction);
	parent.ActionTypeId = ActionTypeId;

	// Parent retrieves his UniqueId from child ParentUniqueId.
	parent.UniqueId = childAction.ParentUniqueId;
	// Parent by default should have both ids same.
	parent.ParentUniqueId = childAction.ParentUniqueId;

	return parent;
}

FUDActionData::FUDActionData(int32 actionTypeId) 
	: ActionTypeId(actionTypeId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId) 
	: ActionTypeId(actionTypeId), 
	InvokerPlayerId(invokerPlayerId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId)
	: ActionTypeId(actionTypeId), 
	InvokerPlayerId(invokerPlayerId), 
	TargetPlayerId(targetPlayerId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId, int32 valueParameter)
	: ActionTypeId(actionTypeId), 
	InvokerPlayerId(invokerPlayerId), 
	TargetPlayerId(targetPlayerId), 
	ValueParameter(valueParameter)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 valueParameter, FIntPoint tileParameter)
	: ActionTypeId(actionTypeId), 
	ValueParameter(valueParameter), 
	TileParameter(tileParameter)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId, FIntPoint tileParameter)
	: ActionTypeId(actionTypeId), 
	InvokerPlayerId(invokerPlayerId), 
	TargetPlayerId(targetPlayerId), 
	TileParameter(tileParameter)
{

}