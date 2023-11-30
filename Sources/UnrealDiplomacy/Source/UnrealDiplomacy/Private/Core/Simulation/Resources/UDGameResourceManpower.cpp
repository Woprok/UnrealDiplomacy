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
			UD_RESOURCE_TAG_TYPE_SECONDARY,
			UD_RESOURCE_TAG_FACTION_STARTING,
			UD_RESOURCE_TAG_TILE_STARTING
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceManpower",
		"Manpower is [{TYPE}] resource."
	)).ToString();

	presentation.FactionStartingAmount = GetFactionStartingAmount();
	presentation.TileStartingAmount = GetTileStartingAmount();
	presentation.TileWeight = GetTileWeight();
	return presentation;
}
#undef LOCTEXT_NAMESPACE