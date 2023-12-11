// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierStratagemActivated.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "TileStratagemActivated"
FUDModifierPresentation UUDTileModifierStratagemActivated::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("TileStratagemActivated", "Tile Stratagem Used")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
			UD_MODIFIER_TAG_PARAMETER_ACTION,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("TileStratagemActivated",
		"Faction [{INVOKER}] activated [{ACTION}] this phase."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE