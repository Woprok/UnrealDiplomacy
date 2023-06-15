// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDTileInteractionViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "TileInteraction"

void UUDTileInteractionViewModel::Initialize()
{
	FText name = FText(LOCTEXT("TileInteraction", "Interaction Name"));
	SetNameText(name);
	FText interact = FText(LOCTEXT("TileInteraction", "Interact"));
	SetInteractText(interact);
}

void UUDTileInteractionViewModel::Update()
{
	FText name = FText::FromString(Content.Name);
	SetNameText(name);
}

#undef LOCTEXT_NAMESPACE

void UUDTileInteractionViewModel::SetContent(FUDTileInteractionInfo content)
{
	Content = content;
}

void UUDTileInteractionViewModel::Interact()
{
	// TODO execution of the action with all current parameters.
	UE_LOG(LogTemp, Log, TEXT("UUDTileInteractionViewModel: Interact."));
	Model->RequestAction(Model->GetAction(Content.ActionTypeId));
}

void UUDTileInteractionViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDTileInteractionViewModel::GetNameText() const
{
	return NameText;
}
void UUDTileInteractionViewModel::SetInteractText(FText newInteractText)
{
	UE_MVVM_SET_PROPERTY_VALUE(InteractText, newInteractText);
}

FText UUDTileInteractionViewModel::GetInteractText() const
{
	return InteractText;
}