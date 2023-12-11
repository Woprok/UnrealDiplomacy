// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDFactionModifierStratagemActivated.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "FactionStratagemActivated"
FUDModifierPresentation UUDFactionModifierStratagemActivated::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("FactionStratagemActivated", "Faction Stratagem Used")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_FACTION,
			UD_MODIFIER_TAG_PARAMETER_ACTION,
		}
	);
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("FactionStratagemActivated",
		"Faction [{INVOKER}] activated [{ACTION}] this phase."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE