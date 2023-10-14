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
	SetIsItemSelectedValue(false);
}

void UUDPolicySelectItemViewModel::Update()
{
	SetSelectItemText(FText::FromString(Content.Name));
}

#undef LOCTEXT_NAMESPACE

void UUDPolicySelectItemViewModel::InvalidateContent(FUDPolicySelectItemInfo content)
{
	Content = content;
	SetIsItemSelectedValue(false);
	Update();
}

void UUDPolicySelectItemViewModel::SetContent(FUDPolicySelectItemInfo content)
{
	Content = content;
	SetIsItemSelectedValue(true);
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

void UUDPolicySelectItemViewModel::SetIsItemSelectedValue(bool newIsItemSelectedValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsItemSelectedValue, newIsItemSelectedValue);
}

bool UUDPolicySelectItemViewModel::GetIsItemSelectedValue() const
{
	return IsItemSelectedValue;
}