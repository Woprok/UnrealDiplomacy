// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDResourceParameterViewModel.h"
#include "Core/UDGlobalData.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishPlayerController.h"

#define LOCTEXT_NAMESPACE "ResourceParameter"

FUDResourceMinimalInfo GetInvalidResource()
{
	FUDResourceMinimalInfo invalid = FUDResourceMinimalInfo();
	invalid.Name = FText(LOCTEXT("ResourceParameter", "Invalid")).ToString();
	invalid.Id = UUDGlobalData::InvalidResourceId;
	return invalid;
}

void UUDResourceParameterViewModel::Initialize()
{
	FText tileTitle = FText(LOCTEXT("ResourceParameter", "Resource"));
	SetResourceTitleText(tileTitle);
	FText previousText = FText(LOCTEXT("ResourceParameter", "<"));
	SetPreviousText(previousText);
	FText nextText = FText(LOCTEXT("ResourceParameter", ">"));
	SetNextText(nextText);
	SelectedResource = GetInvalidResource().Id;
	SetNameText(FText::FromString(GetInvalidResource().Name));
	SelectedResourceIndex = UUDGlobalData::InvalidParameterArrayIndex;
}

void UUDResourceParameterViewModel::Update()
{
	Options = Content.Options;
	SetResourceTitleText(FText::FromString(Content.Name));
	SetToolTipText(FText::FromString(Content.ToolTip));

	FUDResourceMinimalInfo selected = GetSelectedOrDefault(SelectedResource);
	SelectedResource = selected.Id;
	SetNameText(FText::FromString(selected.Name));
}

FUDResourceMinimalInfo UUDResourceParameterViewModel::GetSelectedOrDefault(int32 desiredSelectedItem)
{
	FUDResourceMinimalInfo selected = GetInvalidResource();
	SelectedResourceIndex = UUDGlobalData::InvalidParameterArrayIndex;
	// Find
	const auto& found = Options.FindByPredicate(
		[&desiredSelectedItem](const FUDResourceMinimalInfo& item) { return item.Id == desiredSelectedItem; }
	);
	if (found)
	{
		selected = *found;
		SelectedResourceIndex = Options.Find(selected);
	}
	else if (Options.Num() > 0)
	{
		selected = Options[0];
		SelectedResourceIndex = 0;
	}

	return selected;
}

#undef LOCTEXT_NAMESPACE

void UUDResourceParameterViewModel::PreviousResource()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedResourceIndex - distance >= 0)
	{
		SelectedResourceIndex -= distance;
		FUDResourceMinimalInfo selected = Options[SelectedResourceIndex];
		SelectedResource = selected.Id;
		SetNameText(FText::FromString(selected.Name));
	}
}

void UUDResourceParameterViewModel::NextResource()
{
	int32 distance = AUDSkirmishPlayerController::Get(GetWorld())->GetButtonKeyDistance();
	if (SelectedResourceIndex + distance < Options.Num())
	{
		SelectedResourceIndex += distance;
		FUDResourceMinimalInfo selected = Options[SelectedResourceIndex];
		SelectedResource = selected.Id;
		SetNameText(FText::FromString(selected.Name));
	}
}

void UUDResourceParameterViewModel::SetContent(FUDResourceParameter content)
{
	Content = content;
}

int32 UUDResourceParameterViewModel::GetAsValue()
{
	return SelectedResource;
}

void UUDResourceParameterViewModel::SetResourceTitleText(FText newResourceTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ResourceTitleText, newResourceTitleText);
}

FText UUDResourceParameterViewModel::GetResourceTitleText() const
{
	return ResourceTitleText;
}

void UUDResourceParameterViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDResourceParameterViewModel::GetNameText() const
{
	return NameText;
}

void UUDResourceParameterViewModel::SetToolTipText(FText newToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ToolTipText, newToolTipText);
}

FText UUDResourceParameterViewModel::GetToolTipText() const
{
	return ToolTipText;
}

void UUDResourceParameterViewModel::SetPreviousText(FText newPreviousText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PreviousText, newPreviousText);
}

FText UUDResourceParameterViewModel::GetPreviousText() const
{
	return PreviousText;
}

void UUDResourceParameterViewModel::SetNextText(FText newNextText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NextText, newNextText);
}

FText UUDResourceParameterViewModel::GetNextText() const
{
	return NextText;
}