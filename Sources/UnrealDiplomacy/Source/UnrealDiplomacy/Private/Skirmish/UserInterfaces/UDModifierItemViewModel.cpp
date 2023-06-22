// Copyright Miroslav Valach


#include "Skirmish/UserInterfaces/UDModifierItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"

#define LOCTEXT_NAMESPACE "ModifierItem"

void UUDModifierItemViewModel::Initialize()
{
	FText name = FText(LOCTEXT("ModifierItem", "Modifier"));
	SetNameText(name);
	FText description = FText(LOCTEXT("ModifierItem", "Modifier"));
	SetDescriptionText(description);
	SetIdValue(0);
}

void UUDModifierItemViewModel::Update()
{
	SetNameText(FText::FromString(Content.Name));
	SetDescriptionText(FText::FromString(Content.Description));
	SetIdValue(Content.Id);
}

#undef LOCTEXT_NAMESPACE

void UUDModifierItemViewModel::SetContent(FUDModifierInfo content)
{
	Content = content;
}

void UUDModifierItemViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDModifierItemViewModel::GetNameText() const
{
	return NameText;
}

void UUDModifierItemViewModel::SetDescriptionText(FText newDescriptionText)
{
	UE_MVVM_SET_PROPERTY_VALUE(DescriptionText, newDescriptionText);
}

FText UUDModifierItemViewModel::GetDescriptionText() const
{
	return DescriptionText;
}

void UUDModifierItemViewModel::SetIdValue(int32 newIdValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IdValue, newIdValue);
}

int32 UUDModifierItemViewModel::GetIdValue() const
{
	return IdValue;
}