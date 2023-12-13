// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierBuildingPalace.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "BuildingPalace"
FUDModifierPresentation UUDTileModifierBuildingPalace::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("BuildingPalace", "Building Palace")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("BuildingPalace",
		"This tile contains Palace."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE