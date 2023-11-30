// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDFactionParameterViewModel.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishPlayerController.h"

#define LOCTEXT_NAMESPACE "FactionParameter"

FUDFactionMinimalInfo GetInvalidFaction()
{
	FUDFactionMinimalInfo invalid = FUDFactionMinimalInfo();
	invalid.Name = FText(LOCTEXT("FactionParameter", "Invalid")).ToString();
	invalid.Id = UUDGlobalData::InvalidFactionId;
	return invalid;
}

void UUDFactionParameterViewModel::Setup()
{
	FText tileTitle = FText(LOCTEXT("FactionParameter", "Faction"));
	SetFactionTitleText(tileTitle);
	FText previousText = FText(LOCTEXT("FactionParameter", "<"));
	SetPreviousText(previousText);
	FText nextText = FText(LOCTEXT("FactionParameter", ">"));
	SetNextText(nextText);
	SelectedFaction = GetInvalidFaction().Id;
	SetNameText(FText::FromString(GetInvalidFaction().Name));
	SelectedFactionIndex = UUDGlobalData::InvalidParameterArrayIndex;
}

void UUDFactionParameterViewModel::Refresh()
{
	SetFactionTitleText(FText::FromString(Content.Name));
	SetToolTipText(FText::FromString(Content.ToolTip));

	FUDFactionMinimalInfo selected;
	if (Content.HasCurrentValue)
	{
		selected = GetSelectedOrDefault(Content.CurrentValue);
	}
	else
	{
		selected = GetSelectedOrDefault(SelectedFaction);
	}

	SelectedFaction = selected.Id;
	SetNameText(FText::FromString(selected.Name));
	ChangeAttempted();
}

void UUDFactionParameterViewModel::ChangeAttempted()
{
	if (Content.HasCurrentValue && Content.CurrentValue != GetAsValue())
	{
		OnChangeEvent.Broadcast();
	}
}

FUDFactionMinimalInfo UUDFactionParameterViewModel::GetSelectedOrDefault(int32 desiredSelectedItem)
{
	FUDFactionMinimalInfo selected = GetInvalidFaction();
	SelectedFactionIndex = UUDGlobalData::InvalidParameterArrayIndex;
	// Find
	const auto& found = Content.Options.FindByPredicate(
		[&desiredSelectedItem](const FUDFactionMinimalInfo& item) { return item.Id == desiredSelectedItem; }
	);
	if (found)
	{
		selected = *found;
		SelectedFactionIndex = Content.Options.Find(selected);
	}
	else if (Content.Options.Num() > 0)
	{
		selected = Content.Options[0];
		SelectedFactionIndex = 0;
	}

	return selected;
}

#undef LOCTEXT_NAMESPACE

void UUDFactionParameterViewModel::PreviousFaction()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedFactionIndex - distance >= 0)
	{
		SelectedFactionIndex -= distance;
		FUDFactionMinimalInfo selected = Content.Options[SelectedFactionIndex];
		SelectedFaction = selected.Id;
		SetNameText(FText::FromString(selected.Name));
		ChangeAttempted();
	}
}

void UUDFactionParameterViewModel::NextFaction()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedFactionIndex + distance < Content.Options.Num())
	{
		SelectedFactionIndex += distance;
		FUDFactionMinimalInfo selected = Content.Options[SelectedFactionIndex];
		SelectedFaction = selected.Id;
		SetNameText(FText::FromString(selected.Name));
		ChangeAttempted();
	}
}

void UUDFactionParameterViewModel::SetContent(FUDFactionParameter content)
{
	Content = content;
}

int32 UUDFactionParameterViewModel::GetAsValue()
{
	return SelectedFaction;
}

void UUDFactionParameterViewModel::SetFactionTitleText(FText newFactionTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(FactionTitleText, newFactionTitleText);
}

FText UUDFactionParameterViewModel::GetFactionTitleText() const
{
	return FactionTitleText;
}

void UUDFactionParameterViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDFactionParameterViewModel::GetNameText() const
{
	return NameText;
}

void UUDFactionParameterViewModel::SetToolTipText(FText newToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ToolTipText, newToolTipText);
}

FText UUDFactionParameterViewModel::GetToolTipText() const
{
	return ToolTipText;
}

void UUDFactionParameterViewModel::SetPreviousText(FText newPreviousText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PreviousText, newPreviousText);
}

FText UUDFactionParameterViewModel::GetPreviousText() const
{
	return PreviousText;
}

void UUDFactionParameterViewModel::SetNextText(FText newNextText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NextText, newNextText);
}

FText UUDFactionParameterViewModel::GetNextText() const
{
	return NextText;
}