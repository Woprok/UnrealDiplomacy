// Copyright Miroslav Valach

#include "Skirmish/UserInterfaces/UDResourceItemViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Skirmish/UDSkirmishHUD.h"

#define LOCTEXT_NAMESPACE "ResourceItem"

void UUDResourceItemViewModel::Initialize()
{
	FText name = FText(LOCTEXT("ResourceItem", "Resource"));
	SetNameText(name);
	SetAmountValue(0);
	SetIdValue(0);
}

void UUDResourceItemViewModel::Update()
{
	FText name = FText::FromString(Content.Name);
	SetNameText(name);
	SetAmountValue(Content.Amount);
	SetIdValue(Content.Id);
}

#undef LOCTEXT_NAMESPACE

void UUDResourceItemViewModel::SetContent(FUDResourceInfo content)
{
	Content = content;
}

void UUDResourceItemViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDResourceItemViewModel::GetNameText() const
{
	return NameText;
}

void UUDResourceItemViewModel::SetAmountValue(int32 newAmountValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(AmountValue, newAmountValue);
}

int32 UUDResourceItemViewModel::GetAmountValue() const
{
	return AmountValue;
}

void UUDResourceItemViewModel::SetIdValue(int32 newIdValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IdValue, newIdValue);
}

int32 UUDResourceItemViewModel::GetIdValue() const
{
	return IdValue;
}