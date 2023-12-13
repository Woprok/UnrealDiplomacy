// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDFactionModifierStratagemShare.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "StratagemShare"
FUDModifierPresentation UUDFactionModifierStratagemShare::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("StratagemShare", "Stratagem Share")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_FACTION,
			UD_MODIFIER_TAG_PARAMETER_ACTION,
		}
	);
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("StratagemShare",
		"Faction [{INVOKER}] shares stratagem [{ACTION}] with [{TARGET}]."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE