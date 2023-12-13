// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierBuildingFarm.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "BuildingFarm"
FUDModifierPresentation UUDTileModifierBuildingFarm::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("BuildingFarm", "Building Farm")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("BuildingFarm",
		"This tile contains Farm."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE