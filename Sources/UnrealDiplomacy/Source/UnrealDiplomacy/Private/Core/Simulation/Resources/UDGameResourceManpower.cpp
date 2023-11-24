// Copyright Miroslav Valach

#include "Core/Simulation/Resources/UDGameResourceManpower.h"

#define LOCTEXT_NAMESPACE "ResourceManpower"
FUDResourcePresentation UUDGameResourceManpower::GetPresentation() const
{
	FUDResourcePresentation presentation = FUDResourcePresentation();
	presentation.ResourceId = GetId();
	presentation.Name = FText(LOCTEXT("ResourceManpower", "Manpower")).ToString();
	presentation.Tags.Append(
		{
			UD_RESOURCE_TAG_VALID,
			UD_RESOURCE_TAG_TYPE_SECONDARY
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceManpower",
		"Manpower is [{TYPE}] resource."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE