// Copyright Miroslav Valach

#include "Core/Simulation/Resources/UDGameResourceFood.h"

#define LOCTEXT_NAMESPACE "ResourceFood"
FUDResourcePresentation UUDGameResourceFood::GetPresentation() const
{
	FUDResourcePresentation presentation = FUDResourcePresentation();
	presentation.ResourceId = GetId();
	presentation.Name = FText(LOCTEXT("ResourceFood", "Food")).ToString();
	presentation.Tags.Append(
		{
			UD_RESOURCE_TAG_VALID,
			UD_RESOURCE_TAG_TYPE_SECONDARY
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceFood",
		"Food is [{TYPE}] resource."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE