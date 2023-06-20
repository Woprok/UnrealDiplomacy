// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/Parameters/UDValueParameterViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"

#define LOCTEXT_NAMESPACE "ValueParameter"

void UUDValueParameterViewModel::Initialize()
{

}

void UUDValueParameterViewModel::Update()
{
	SetMaxValue(Content.MaxValue);
	SetMinValue(Content.MinValue);
	SetValueTitleText(FText::FromString(Content.Name));
	SetToolTipText(FText::FromString(Content.ToolTip));
	// Basically if user is editing the value, we will ignore it during the update.
	if (IsValueEditing)
	{
		return;
	}
	// But if user is not editing the value we will clamp it into the bounds by force.
	int32 resetValue = FMath::Clamp(GetSelectedValue(), Content.MinValue, Content.MaxValue);
	SetSelectedValue(resetValue);
}

#undef LOCTEXT_NAMESPACE

void UUDValueParameterViewModel::SetContent(FUDValueParameter content)
{
	Content = content;
}

int32 UUDValueParameterViewModel::GetAsValue()
{
	return GetSelectedValue();
}

void UUDValueParameterViewModel::StartValueEditation(float InValue)
{
	IsValueEditing = true;
}

void UUDValueParameterViewModel::StopValueEditation(float InValue, ETextCommit::Type CommitMethod)
{
	IsValueEditing = false;
	// Update only if value was changed.
	int32 newValue = InValue;
	int32 oldValue = GetSelectedValue();
	// TODO find source of the bug that causes the binding for SpinBox to not update automatically.
	if (oldValue != newValue)
	{
		SetSelectedValue(newValue);
	}
}

void UUDValueParameterViewModel::SetValueTitleText(FText newValueTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ValueTitleText, newValueTitleText);
}

FText UUDValueParameterViewModel::GetValueTitleText() const
{
	return ValueTitleText;
}

void UUDValueParameterViewModel::SetToolTipText(FText newToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ToolTipText, newToolTipText);
}

FText UUDValueParameterViewModel::GetToolTipText() const
{
	return ToolTipText;
}

void UUDValueParameterViewModel::SetMinValue(float newMinValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MinValue, newMinValue);
}

float UUDValueParameterViewModel::GetMinValue() const
{
	return MinValue;
}

void UUDValueParameterViewModel::SetMaxValue(float newMaxValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxValue, newMaxValue);
}

float UUDValueParameterViewModel::GetMaxValue() const
{
	return MaxValue;
}

void UUDValueParameterViewModel::SetSelectedValue(float newSelectedValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectedValue, newSelectedValue);
}

float UUDValueParameterViewModel::GetSelectedValue() const
{
	return SelectedValue;
}