// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDFactionModifierThroneSupport.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "ThroneSupport"
FUDModifierPresentation UUDFactionModifierThroneSupport::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("ThroneSupport", "Throne Support")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_FACTION,
		}
	);
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("ThroneSupport",
		"Faction [{INVOKER}] supports faction [{TARGET}] to ascend."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE