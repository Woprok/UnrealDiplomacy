// Copyright Miroslav Valach

#include "Core/Simulation/UDActionData.h"

FUDActionData::FUDActionData()
{

}

FUDActionData::FUDActionData(const FUDActionData& existingAction)
	: 
	ActionTypeId(existingAction.ActionTypeId),
	UniqueId(existingAction.UniqueId),
	SourceId(existingAction.SourceId),
	InvokerFactionId(existingAction.InvokerFactionId),
	ValueParameters(existingAction.ValueParameters),
	TextParameter(existingAction.TextParameter)
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

FUDActionData FUDActionData::FromValues(const TArray<int32>& values)
{
	FUDActionData action = FUDActionData();
	action.ActionTypeId = values[0];
	action.UniqueId = values[1];
	action.SourceId = values[2];
	action.InvokerFactionId = values[3];
	for (int32 i = 4; i < values.Num(); i++)
	{
		action.ValueParameters.Add(values[i]);
	}
	return action;
}

TArray<int32> FUDActionData::ToValues(const FUDActionData& action)
{
	TArray<int32> data = { };
	data.Add(action.ActionTypeId);
	data.Add(action.UniqueId);
	data.Add(action.SourceId);
	data.Add(action.InvokerFactionId);
	data.Append(action.ValueParameters);
	return data;
}

FUDActionData FUDActionData::AsSuccessorOf(const FUDActionData& parentAction, int32 ActionTypeId)
{
	FUDActionData child = FUDActionData::CreateDataCopy(parentAction);
	child.ActionTypeId = ActionTypeId;

	// child inherits UniqueId as ParentUniqueId
	child.SourceId = parentAction.UniqueId;
	// child.UniqueId remains default value 0, so it can be assigned by WorldSimulation

	return child;
}

FUDActionData FUDActionData::AsPredecessorOf(const FUDActionData& childAction, int32 ActionTypeId)
{
	FUDActionData parent = FUDActionData::CreateDataCopy(childAction);
	parent.ActionTypeId = ActionTypeId;

	// Parent retrieves his UniqueId from child ParentUniqueId.
	parent.UniqueId = childAction.SourceId;
	// Parent by default should have both ids same.
	parent.SourceId = childAction.SourceId;

	return parent;
}

FString FUDActionData::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	formatArgs.Add(TEXT("aid"), ActionTypeId);
	formatArgs.Add(TEXT("uid"), UniqueId);
	formatArgs.Add(TEXT("sid"), SourceId);
	formatArgs.Add(TEXT("invoker"), InvokerFactionId);
	formatArgs.Add(TEXT("values"), ValueParameters.Num());
	formatArgs.Add(TEXT("backup"), BackupValueParameters.Num());
	formatArgs.Add(TEXT("text"), TextParameter.Len());
	formatArgs.Add(TEXT("backupText"), BackupTextParameter.Len());

	FString formatted = FString::Format(TEXT("Action[ID={aid}, UID={uid}, SID={sid}, INVOKER={invoker}, \
OPTIONAL={values}, {backup}, {text}, {backupText}]"), formatArgs);

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