// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDActionParameterViewModel.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishPlayerController.h"

#define LOCTEXT_NAMESPACE "ActionParameter"

FUDActionMinimalInfo GetInvalidAction()
{
	FUDActionMinimalInfo invalid = FUDActionMinimalInfo();
	invalid.Name = FText(LOCTEXT("ActionParameter", "Invalid")).ToString();
	invalid.Id = UUDGlobalData::InvalidActionId;
	return invalid;
}

void UUDActionParameterViewModel::Initialize()
{
	FText tileTitle = FText(LOCTEXT("ActionParameter", "Action"));
	SetActionTitleText(tileTitle);
	FText previousText = FText(LOCTEXT("ActionParameter", "<"));
	SetPreviousText(previousText);
	FText nextText = FText(LOCTEXT("ActionParameter", ">"));
	SetNextText(nextText);
	SelectedAction = GetInvalidAction().Id;
	SetNameText(FText::FromString(GetInvalidAction().Name));
	SelectedActionIndex = UUDGlobalData::InvalidParameterArrayIndex;
}

void UUDActionParameterViewModel::Update()
{
	SetActionTitleText(FText::FromString(Content.Name));
	SetToolTipText(FText::FromString(Content.ToolTip));

	FUDActionMinimalInfo selected = GetSelectedOrDefault(SelectedAction);
	SelectedAction = selected.Id;
	SetNameText(FText::FromString(selected.Name));
	OnChangeEvent.Broadcast();
}

FUDActionMinimalInfo UUDActionParameterViewModel::GetSelectedOrDefault(int32 desiredSelectedItem)
{
	FUDActionMinimalInfo selected = GetInvalidAction();
	SelectedActionIndex = UUDGlobalData::InvalidParameterArrayIndex;
	// Find
	const auto& found = Content.Options.FindByPredicate(
		[&desiredSelectedItem](const FUDActionMinimalInfo& item) { return item.Id == desiredSelectedItem; }
	);
	if (found)
	{
		selected = *found;
		SelectedActionIndex = Content.Options.Find(selected);
	}
	else if (Content.Options.Num() > 0)
	{
		selected = Content.Options[0];
		SelectedActionIndex = 0;
	}

	return selected;
}

#undef LOCTEXT_NAMESPACE

void UUDActionParameterViewModel::PreviousAction()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedActionIndex - distance >= 0)
	{
		SelectedActionIndex -= distance;
		FUDActionMinimalInfo selected = Content.Options[SelectedActionIndex];
		SelectedAction = selected.Id;
		SetNameText(FText::FromString(selected.Name));
		OnChangeEvent.Broadcast();
	}
}

void UUDActionParameterViewModel::NextAction()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedActionIndex + distance < Content.Options.Num())
	{
		SelectedActionIndex += distance;
		FUDActionMinimalInfo selected = Content.Options[SelectedActionIndex];
		SelectedAction = selected.Id;
		SetNameText(FText::FromString(selected.Name));
		OnChangeEvent.Broadcast();
	}
}

void UUDActionParameterViewModel::SetContent(FUDActionParameter content)
{
	Content = content;
}

int32 UUDActionParameterViewModel::GetAsValue()
{
	return SelectedAction;
}

void UUDActionParameterViewModel::SetActionTitleText(FText newActionTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActionTitleText, newActionTitleText);
}

FText UUDActionParameterViewModel::GetActionTitleText() const
{
	return ActionTitleText;
}

void UUDActionParameterViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDActionParameterViewModel::GetNameText() const
{
	return NameText;
}

void UUDActionParameterViewModel::SetToolTipText(FText newToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ToolTipText, newToolTipText);
}

FText UUDActionParameterViewModel::GetToolTipText() const
{
	return ToolTipText;
}

void UUDActionParameterViewModel::SetPreviousText(FText newPreviousText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PreviousText, newPreviousText);
}

FText UUDActionParameterViewModel::GetPreviousText() const
{
	return PreviousText;
}

void UUDActionParameterViewModel::SetNextText(FText newNextText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NextText, newNextText);
}

FText UUDActionParameterViewModel::GetNextText() const
{
	return NextText;
}