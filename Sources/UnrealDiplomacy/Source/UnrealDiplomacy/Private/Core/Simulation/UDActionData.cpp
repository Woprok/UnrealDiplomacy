// Copyright Miroslav Valach

#include "Core/Simulation/UDActionData.h"

FUDActionData::FUDActionData()
{

}

FUDActionData::FUDActionData(const FUDActionData& action) 
	: 
	ActionTypeId(action.ActionTypeId),
	UniqueId(action.UniqueId),
	SourceUniqueId(action.SourceUniqueId),
	InvokerFactionId(action.InvokerFactionId),
	ValueParameters(action.ValueParameters),
	TextParameter(action.TextParameter)
{

}

FUDActionData FUDActionData::CreateDataCopy(const FUDActionData& existingAction)
{
	FUDActionData action = FUDActionData();
	action.InvokerFactionId = existingAction.InvokerFactionId;
	action.ValueParameters = existingAction.ValueParameters;
	action.TextParameter = existingAction.TextParameter;
	return action;
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

FString FUDActionData::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	formatArgs.Add(TEXT("aid"), ActionTypeId);
	formatArgs.Add(TEXT("uid"), UniqueId);
	formatArgs.Add(TEXT("sid"), SourceUniqueId);
	formatArgs.Add(TEXT("invoker"), InvokerFactionId);
	formatArgs.Add(TEXT("values"), ValueParameters.Num());
	formatArgs.Add(TEXT("backup"), BackupValueParameters.Num());
	formatArgs.Add(TEXT("text"), TextParameter.Len());

	FString formatted = FString::Format(TEXT("Action[ID={aid}, UID={uid}, SID={sid}, INVOKER={invoker}, \
OPTIONAL={values},{backup},{text}]"), formatArgs);

	return formatted;
}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId)
	: ActionTypeId(actionTypeId), InvokerFactionId(invokerPlayerId)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, TArray<int32> valueParameters)
	: ActionTypeId(actionTypeId), InvokerFactionId(invokerPlayerId), ValueParameters(valueParameters)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, FString textParameter)
	: ActionTypeId(actionTypeId), InvokerFactionId(invokerPlayerId), TextParameter(textParameter)
{

}

FUDActionData::FUDActionData(int32 actionTypeId, int32 invokerPlayerId, TArray<int32> valueParameters, FString textParameter)
	: ActionTypeId(actionTypeId), InvokerFactionId(invokerPlayerId), ValueParameters(valueParameters), TextParameter(textParameter)
{

}