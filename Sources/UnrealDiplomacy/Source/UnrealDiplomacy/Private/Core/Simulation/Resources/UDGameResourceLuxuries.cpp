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
			UD_RESOURCE_TAG_TYPE_SECONDARY,
			UD_RESOURCE_TAG_FACTION_STARTING,
			UD_RESOURCE_TAG_TILE_STARTING
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceLuxuries",
		"Luxuries are [{TYPE}] resource."
	)).ToString();

	presentation.FactionStartingAmount = GetFactionStartingAmount();
	presentation.TileStartingAmount = GetTileStartingAmount();
	presentation.TileWeight = GetTileWeight();
	return presentation;
}
#undef LOCTEXT_NAMESPACE