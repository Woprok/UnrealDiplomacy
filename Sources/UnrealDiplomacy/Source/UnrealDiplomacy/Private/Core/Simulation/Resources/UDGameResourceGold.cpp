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
			UD_RESOURCE_TAG_TYPE_PRIMARY,
			UD_RESOURCE_TAG_FACTION_STARTING,
			UD_RESOURCE_TAG_TILE_STARTING
		}
	);
	// TODO what is actually a resource description format ?
	presentation.ResourceDescriptionFormat = FText(LOCTEXT("ResourceGold",
		"Gold is [{TYPE}] resource."
	)).ToString();

	presentation.FactionStartingAmount = GetFactionStartingAmount();
	presentation.TileStartingAmount = GetTileStartingAmount();
	presentation.TileWeight = GetTileWeight();
	return presentation;
}
#undef LOCTEXT_NAMESPACE