// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierBuildingFortress.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "BuildingFortress"
FUDModifierPresentation UUDTileModifierBuildingFortress::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("BuildingFortress", "Building Fortress")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("BuildingFortress",
		"This tile contains Fortress."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE