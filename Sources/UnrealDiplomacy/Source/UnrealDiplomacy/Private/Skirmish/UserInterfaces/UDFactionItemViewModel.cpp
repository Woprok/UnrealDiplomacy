// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDFactionItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "FactionItem"

void UUDFactionItemViewModel::Initialize()
{
	FText name = FText(LOCTEXT("FactionItem", "Faction"));
	SetNameText(name);
	FText controller = FText(LOCTEXT("FactionItem", "[AI]"));
	SetControllerText(controller);
	SetIsHumanValue(false);
}

void UUDFactionItemViewModel::Update()
{
	FText name = FText::FromString(Content.Name);
	SetNameText(name);
	FText controller;
	if (Content.IsPlayerControlled)
		controller = FText(LOCTEXT("FactionItem", "[PC]"));
	else
		controller = FText(LOCTEXT("FactionItem", "[AI]"));
	SetControllerText(controller);
	SetIsHumanValue(Content.IsPlayerControlled);
}

#undef LOCTEXT_NAMESPACE

void UUDFactionItemViewModel::SetContent(FUDFactionInfo content)
{
	Content = content;
}

void UUDFactionItemViewModel::Interact()
{
	AUDSkirmishHUD::Get(GetWorld())->RequestNotifyOnFactionSelected(Content.Id);
}

void UUDFactionItemViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDFactionItemViewModel::GetNameText() const
{
	return NameText;
}

void UUDFactionItemViewModel::SetControllerText(FText newControllerText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ControllerText, newControllerText);
}

FText UUDFactionItemViewModel::GetControllerText() const
{
	return ControllerText;
}

void UUDFactionItemViewModel::SetIsHumanValue(bool newIsHumanValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsHumanValue, newIsHumanValue);
}

bool UUDFactionItemViewModel::GetIsHumanValue() const
{
	return IsHumanValue;
}