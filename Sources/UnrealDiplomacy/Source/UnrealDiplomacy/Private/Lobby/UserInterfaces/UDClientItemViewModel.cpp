// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDClientItemViewModel.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "ClientItem"

void UUDClientItemViewModel::Setup()
{
	FText name = FText(LOCTEXT("ClientItem", "Client"));
	SetNameText(name);
}

void UUDClientItemViewModel::Refresh()
{
	SetNameText(FText::FromString(Content.Name));
}

#undef LOCTEXT_NAMESPACE

void UUDClientItemViewModel::SetContent(FUDFactionMinimalInfo content)
{
	Content = content;
}

void UUDClientItemViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDClientItemViewModel::GetNameText() const
{
	return NameText;
}