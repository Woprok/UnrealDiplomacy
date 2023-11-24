// Copyright Miroslav Valach

#include "Core/Simulation/UDResourceInterface.h"
#include "Core/UDGlobalData.h"

int32 IUDResourceInterface::GetId() const
{
	return UUDGlobalData::InvalidResourceId;
}

#define LOCTEXT_NAMESPACE "ResourceInterface"
FUDResourcePresentation IUDResourceInterface::GetPresentation() const
{
	FUDResourcePresentation presentation = FUDResourcePresentation();
	presentation.ResourceId = GetId();
	presentation.Name = FText(LOCTEXT("ResourceInterface", "Invalid Resource")).ToString();
	presentation.Tags.Append(
		{
			UD_RESOURCE_TAG_INVALID,
		}
	);

	return presentation;
}
#undef LOCTEXT_NAMESPACE

FString IUDResourceInterface::ToString() const
{
	FStringFormatNamedArguments formatArgs;
	formatArgs.Add(TEXT("rid"), GetId());

	FString formatted = FString::Format(TEXT("Resource Executor[ID={rid}, OPTIONAL=NONE]"), formatArgs);

	return formatted;
}