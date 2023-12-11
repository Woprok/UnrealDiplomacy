// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDFactionModifierMilitarySupport.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "MilitarySupport"
FUDModifierPresentation UUDFactionModifierMilitarySupport::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("MilitarySupport", "Military Support")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_FACTION,
		}
	);
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("MilitarySupport",
		"Faction [{INVOKER}] pledges army to faction [{TARGET}] cause."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE