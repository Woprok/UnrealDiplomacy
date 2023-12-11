// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierStratagemUsed.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "TileStratagemUsed"
FUDModifierPresentation UUDTileModifierStratagemUsed::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("TileStratagemUsed", "Tile Stratagem Used")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("TileStratagemUsed",
		"Faction [{INVOKER}] gave faction [{TARGET}] permit to exploit this tile."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE