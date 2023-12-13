// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierBuildingQuarry.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "BuildingQuarry"
FUDModifierPresentation UUDTileModifierBuildingQuarry::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("BuildingQuarry", "Building Quarry")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("BuildingQuarry",
		"This tile contains Quarry."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE