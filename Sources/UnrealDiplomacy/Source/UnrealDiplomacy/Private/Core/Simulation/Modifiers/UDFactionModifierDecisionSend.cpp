// Copyright Miroslav Valach

#include "Core/Simulation/Modifiers/UDFactionModifierDecisionSend.h"
#include "Core/Simulation/UDModifierInterface.h"

#define LOCTEXT_NAMESPACE "DecisionSend"
FUDModifierPresentation UUDFactionModifierDecisionSend::GetPresentation() const
{
	FUDModifierPresentation presentation = FUDModifierPresentation();
	presentation.ModifierId = GetId();
	presentation.Name = FText(LOCTEXT("DecisionSend", "Decision Send")).ToString();
	presentation.Tags.Append(
		{
			UD_MODIFIER_TAG_VALID,
			UD_MODIFIER_TAG_TYPE_FACTION,
			UD_MODIFIER_TAG_PARAMETER_ACTION,
		}
	);
	presentation.ModifierDescriptionFormat = FText(LOCTEXT("DecisionSend",
		"Faction [{INVOKER}] can't send another decision with [{ACTION}] as resolution."
	)).ToString();

	return presentation;
}
#undef LOCTEXT_NAMESPACE