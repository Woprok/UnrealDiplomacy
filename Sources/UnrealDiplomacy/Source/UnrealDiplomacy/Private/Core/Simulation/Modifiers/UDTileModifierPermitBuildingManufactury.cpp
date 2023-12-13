// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierBuildingManufactury.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "BuildingManufactury"
FUDModifierPresentation UUDTileModifierBuildingManufactury::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("BuildingManufactury", "Building Manufactury")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("BuildingManufactury",
		"This tile contains Manufactury."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE