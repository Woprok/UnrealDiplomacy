// Copyright Miroslav Valach

#include "Core/Simulation/UDActionData.h"

FUDActionData::FUDActionData()
{

}

FUDActionData::FUDActionData(const FUDActionData& actionData) 
	: 
	ActionTypeId(actionData.ActionTypeId),
	InvokerPlayerId(actionData.InvokerPlayerId),
	UniqueId(actionData.UniqueId),
	SourceUniqueId(actionData.SourceUniqueId),
	ValueParameters(actionData.ValueParameters),
	TextParameter(actionData.TextParameter)
{

}

FUDActionData FUDActionData::CreateDataCopy(const FUDActionData& existingAction)
{
	FUDActionData actionData = FUDActionData();
	actionData.InvokerPlayerId = existingAction.InvokerPlayerId;
	actionData.ValueParameters = existingAction.ValueParameters;
	actionData.TextParameter = existingAction.TextParameter;
	return actionData;
}

//FUDActionData FUDActionData::AsChildOf(const FUDActionData& parentAction, int32 parentUniqueId)
//{
//	FUDActionData child = FUDActionData::CreateDataCopy(parentAction);
//	child.ActionTypeId = parentAction.ActionTypeId;
//
//	// child inherits UniqueId as ParentUniqueId
//	child.SourceUniqueId = parentUniqueId;
//	// child.UniqueId remains default value 0, so it can be assigned by WorldSimulation
//
//	return child;
//}

FUDActionData FUDActionData::AsSuccessorOf(const FUDActionData& parentAction, int32 ActionTypeId)
{
	FUDActionData child = FUDActionData::CreateDataCopy(parentAction);
	child.ActionTypeId = ActionTypeId;

	// child inherits UniqueId as ParentUniqueId
	child.SourceUniqueId = parentAction.UniqueId;
	// child.UniqueId remains default value 0, so it can be assigned by WorldSimulation

	return child;
}

FUDActionData FUDActionData::AsPredecessorOf(const FUDActionData& childAction, int32 ActionTypeId)
{
	FUDActionData parent = FUDActionData::CreateDataCopy(childAction);
	parent.ActionTypeId = ActionTypeId;

	// Parent retrieves his UniqueId from child ParentUniqueId.
	parent.UniqueId = childAction.SourceUniqueId;
	// Parent by default should have both ids same.
	parent.SourceUniqueId = childAction.SourceUniqueId;

	return parent;
}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId)
	: ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, TArray<int32> valueParameters)
	: ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId), ValueParameters(valueParameters)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, FString textParameter)
	: ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId), TextParameter(textParameter)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, TArray<int32> valueParameters, FString textParameter)
	: ActionTypeId(actionTypeId), InvokerPlayerId(invokerPlayerId), ValueParameters(valueParameters), TextParameter(textParameter)
{

}