// Copyright Miroslav Valach

#include "Core/Simulation/UDModifierData.h"
#include "Core/UDGlobalData.h"

FUDModifierData::FUDModifierData()
{

}

FUDModifierData::FUDModifierData(const FUDModifierData& existingModifier)
	:
	ModifierTypeId(existingModifier.ModifierTypeId),
	ActionUniqueId(existingModifier.ActionUniqueId),
	InvokerId(existingModifier.InvokerId),
	TargetId(existingModifier.TargetId),
	ValueParameters(existingModifier.ValueParameters)
{

}

FUDModifierData::FUDModifierData(int32 modifierTypeId, int32 actionUniqueId, int32 creatorId, int32 targetId)
	: ModifierTypeId(modifierTypeId), ActionUniqueId(actionUniqueId), InvokerId(creatorId), TargetId(targetId)
{

}

FUDModifierData::FUDModifierData(int32 modifierTypeId, int32 actionUniqueId, int32 creatorId, int32 targetId, TArray<int32> valueParameters)
	: ModifierTypeId(modifierTypeId), ActionUniqueId(actionUniqueId), InvokerId(creatorId), TargetId(targetId), ValueParameters(valueParameters)
{

}

FString FUDModifierData::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	formatArgs.Add(TEXT("mid"), ModifierTypeId);
	formatArgs.Add(TEXT("auid"), ActionUniqueId);
	formatArgs.Add(TEXT("invoker"), InvokerId);
	formatArgs.Add(TEXT("target"), TargetId);
	formatArgs.Add(TEXT("values"), ValueParameters.Num());

	FString formatted = FString::Format(TEXT("Modifier[ID={mid}, AUID={auid}, InvokerId={invoker}, TargetId={target}, \
OPTIONAL={values}]"), formatArgs);

	return formatted;
}