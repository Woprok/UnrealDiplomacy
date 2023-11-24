// Copyright Miroslav Valach

#include "Core/Simulation/Resources/UDGameResourceReputation.h"

#define LOCTEXT_NAMESPACE "ResourceReputation"
FUDResourcePresentation UUDGameResourceReputation::GetPresentation() const
{
	FUDResourcePresentation presentation = FUDResourcePresentation();
	presentation.ResourceId = GetId();
	presentation.Name = FText(LOCTEXT("ResourceReputation", "Reputation")).ToString();
	presentation.Tags.Append(
		{
			UD_RESOURCE_TAG_VALID,
			UD_RESOURCE_TAG_TYPE_PRIMARY
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceReputation",
		"Reputation is [{TYPE}] resource. Used as one of the victory conditions."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE