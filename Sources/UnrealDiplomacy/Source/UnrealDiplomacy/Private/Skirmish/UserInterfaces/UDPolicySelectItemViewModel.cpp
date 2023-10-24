// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDPolicySelectItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/UDActionData.h"

#define LOCTEXT_NAMESPACE "PolicyItem"

void UUDPolicySelectItemViewModel::Initialize()
{
	FText selectItem = FText(LOCTEXT("PolicyItem", "Undefined"));
	SetSelectItemText(selectItem);
}

void UUDPolicySelectItemViewModel::Update()
{
	SetSelectItemText(FText::FromString(Content.Name));
	SetCanSelectItemValue(!Content.IsSelected);
}

#undef LOCTEXT_NAMESPACE

void UUDPolicySelectItemViewModel::SetContent(FUDPolicySelectItemInfo content)
{
	Content = content;
	// Can select only these that are not selected.
	Update();
}

void UUDPolicySelectItemViewModel::Select()
{
	UE_LOG(LogTemp, Log, TEXT("UUDPolicySelectItemViewModel: Select."));
	Model->RequestAction(Model->GetAction(Content.SelectPolicyActionId, { Content.PolicyId }));
}

void UUDPolicySelectItemViewModel::SetSelectItemText(FText newSelectItemText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectItemText, newSelectItemText);
}

FText UUDPolicySelectItemViewModel::GetSelectItemText() const
{
	return SelectItemText;
}

void UUDPolicySelectItemViewModel::SetCanSelectItemValue(bool newCanSelectItemValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(CanSelectItemValue, newCanSelectItemValue);
}

bool UUDPolicySelectItemViewModel::GetCanSelectItemValue() const
{
	return CanSelectItemValue;
}