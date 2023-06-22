// Copyright Miroslav Valach

#include "Core/Simulation/UDModifierInterface.h"
#include "Core/UDGlobalData.h"

int32 IUDModifierInterface::GetId() const
{
	return UUDGlobalData::InvalidModifierId;
}

int32 IUDModifierInterface::GetParameterCount() const
{
	return UUDGlobalData::DefaultModifierParameterCount;
}

#define LOCTEXT_NAMESPACE "ModifierInterface"
FUDModifierPresentation IUDModifierInterface::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("ModifierInterface", "Invalid Modifier")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_INVALID,
		}
	);

	return presentation;
}
#undef LOCTEXT_NAMESPACE

FString IUDModifierInterface::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	formatArgs.Add(TEXT("mid"), GetId());
	formatArgs.Add(TEXT("values"), GetParameterCount());

	FString formatted = FString::Format(TEXT("Modifier Executor[ID={mid}, OPTIONAL={values}]"), formatArgs);

	return formatted;
}