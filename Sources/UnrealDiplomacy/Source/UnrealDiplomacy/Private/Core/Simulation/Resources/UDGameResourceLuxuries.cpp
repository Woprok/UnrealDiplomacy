// Copyright Miroslav Valach

#include "Core/Simulation/Resources/UDGameResourceLuxuries.h"

#define LOCTEXT_NAMESPACE "ResourceLuxuries"

FUDResourcePresentation UUDGameResourceLuxuries::GetPresentation() const
{
	FUDResourcePresentation presentation = FUDResourcePresentation();
	presentation.ResourceId = GetId();
	presentation.Name = FText(LOCTEXT("ResourceLuxuries", "Luxuries")).ToString();
	presentation.Tags.Append(
		{
			UD_RESOURCE_TAG_VALID,
			UD_RESOURCE_TAG_TYPE_SECONDARY
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceLuxuries",
		"Luxuries are [{TYPE}] resource."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE