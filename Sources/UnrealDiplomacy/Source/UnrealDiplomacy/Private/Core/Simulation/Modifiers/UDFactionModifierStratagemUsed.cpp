// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDFactionModifierStratagemUsed.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "FactionStratagemUsed"
FUDModifierPresentation UUDFactionModifierStratagemUsed::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("FactionStratagemUsed", "Faction Stratagem Used")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_FACTION,
		}
	);
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("FactionStratagemUsed",
		"Faction [{INVOKER}] supports faction [{TARGET}] to ascend."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE