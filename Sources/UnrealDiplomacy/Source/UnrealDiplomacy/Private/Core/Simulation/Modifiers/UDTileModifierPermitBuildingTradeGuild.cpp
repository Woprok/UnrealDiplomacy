// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDTileModifierBuildingTradeGuild.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "BuildingTradeGuild"
FUDModifierPresentation UUDTileModifierBuildingTradeGuild::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("BuildingTradeGuild", "Building Trade Guild")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_TILE,
		}
	);
	// TODO this should include tile ? it's not really needed but it might be better.
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("BuildingTradeGuild",
		"This tile contains Trade Guild."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE