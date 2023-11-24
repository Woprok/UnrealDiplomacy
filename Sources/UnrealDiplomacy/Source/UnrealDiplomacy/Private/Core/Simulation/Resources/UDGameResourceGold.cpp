// Copyright Miroslav Valach

#include "Core/Simulation/Resources/UDGameResourceGold.h"

#define LOCTEXT_NAMESPACE "ResourceGold"
FUDResourcePresentation UUDGameResourceGold::GetPresentation() const
{
	FUDResourcePresentation presentation = FUDResourcePresentation();
	presentation.ResourceId = GetId();
	presentation.Name = FText(LOCTEXT("ResourceGold", "Gold")).ToString();
	presentation.Tags.Append(
		{
			UD_RESOURCE_TAG_VALID,
			UD_RESOURCE_TAG_TYPE_PRIMARY
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceGold",
		"Gold is [{TYPE}] resource."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE