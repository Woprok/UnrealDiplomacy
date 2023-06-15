// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionInteractionViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "FactionInteraction"

void UUDFactionInteractionViewModel::Initialize()
{
	FText name = FText(LOCTEXT("FactionInteraction", "Interaction Name"));
	SetNameText(name);
	FText interact = FText(LOCTEXT("FactionInteraction", "Interact"));
	SetInteractText(interact);
}

void UUDFactionInteractionViewModel::Update()
{
	FText name = FText::FromString(Content.Name);
	SetNameText(name);
}

#undef LOCTEXT_NAMESPACE

void UUDFactionInteractionViewModel::SetContent(FUDFactionInteractionInfo content)
{
	Content = content;
}

void UUDFactionInteractionViewModel::Interact()
{
	// TODO execution of the action with all current parameters.
	UE_LOG(LogTemp, Log, TEXT("UUDFactionInteractionViewModel: Interact."));
	Model->RequestAction(Model->GetAction(Content.ActionTypeId));
}

void UUDFactionInteractionViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDFactionInteractionViewModel::GetNameText() const
{
	return NameText;
}
void UUDFactionInteractionViewModel::SetInteractText(FText newInteractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractText, newInteractText);
}

FText UUDFactionInteractionViewModel::GetInteractText() const
{
	return InteractText;
}