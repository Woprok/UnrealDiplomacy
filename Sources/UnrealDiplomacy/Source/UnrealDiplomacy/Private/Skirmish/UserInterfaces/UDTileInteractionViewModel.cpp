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

void UUDTileInteractionViewModel::SetContent(FIntPoint selectedTile, FUDTileInteractionInfo content)
{
	SelectedTile = selectedTile;
	Content = content;
}

void UUDTileInteractionViewModel::Interact()
{
	UE_LOG(LogTemp, Log, TEXT("UUDTileInteractionViewModel: Interact."));
	// Start with the guaranteed parameter.
	TArray<int32> valueParameters = { };
	valueParameters.Add(SelectedTile.X);
	valueParameters.Add(SelectedTile.Y);
	// TODO execution of the action with all current parameters.

	Model->RequestAction(Model->GetAction(Content.ActionTypeId, valueParameters));
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