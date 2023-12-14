// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDFactionModifierThroneUsurper.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "ThroneUsurper"
FUDModifierPresentation UUDFactionModifierThroneUsurper::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneUsurper", "Throne Usurper")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_FACTION,
		}
	);
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("ThroneUsurper",
		"Faction [{INVOKER}] is unable to usurp the throne for rest of the game."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE