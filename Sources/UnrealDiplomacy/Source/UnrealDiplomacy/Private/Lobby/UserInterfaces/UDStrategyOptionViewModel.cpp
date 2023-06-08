// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDStrategyOptionViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemOptionSelect.h"
#include "Core/Simulation/Actions/UDSettingActionStratagemOptionDeselect.h"

#define LOCTEXT_NAMESPACE "StrategyOption"

void UUDStrategyOptionViewModel::Initialize()
{
	FText optionToolTip = FText(LOCTEXT("StrategyOption", "ToolTip"));
	SetOptionToolTipText(optionToolTip);
	SetIsSelectedValue(false);
}

void UUDStrategyOptionViewModel::Update()
{
	FText optionTooltip = FText::Format(LOCTEXT("StrategyOption", "{0}\nCost: [{1}]"), FText::FromString(Content.Name), Content.Cost);
	SetOptionToolTipText(optionTooltip);
	SetIsSelectedValue(Content.IsSelected);
}

#undef LOCTEXT_NAMESPACE

void UUDStrategyOptionViewModel::Selected()
{
	// Manual detection of select and deselect...
	if (!GetIsSelectedValue())
	{
		// Check cost to prevent server from killing the check.
		if (!Model->IsStratagemTakeable(Content))
			return;
		Model->RequestAction(Model->GetAction(UUDSettingActionStratagemOptionSelect::ActionTypeId, { Content.Id }));
	}
	else
	{
		Model->RequestAction(Model->GetAction(UUDSettingActionStratagemOptionDeselect::ActionTypeId, { Content.Id }));
	}
}

void UUDStrategyOptionViewModel::SetContent(FUDStratagemPickableInfo content)
{
	Content = content;
}

void UUDStrategyOptionViewModel::SetOptionToolTipText(FText newOptionToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(OptionToolTipText, newOptionToolTipText);
}

FText UUDStrategyOptionViewModel::GetOptionToolTipText() const
{
	return OptionToolTipText;
}

void UUDStrategyOptionViewModel::SetIsSelectedValue(bool newIsSelectedValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsSelectedValue, newIsSelectedValue);
}

bool UUDStrategyOptionViewModel::GetIsSelectedValue() const
{
	return IsSelectedValue;
}