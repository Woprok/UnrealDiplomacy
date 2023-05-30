// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDClientItemViewModel.h"

#define LOCTEXT_NAMESPACE "ClientItem"

void UUDClientItemViewModel::Initialize()
{
	FText name = FText(LOCTEXT("ClientItem", "Client"));
	SetNameText(name);
	SetIsHostValue(false);
}

void UUDClientItemViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDClientItemViewModel::SetContent(UObject* content)
{
	Content = content;
}

const UObject* UUDClientItemViewModel::GetContent() const
{
	return Content;
}

void UUDClientItemViewModel::SetNameText(FText newNameText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameText, newNameText);
}

FText UUDClientItemViewModel::GetNameText() const
{
	return NameText;
}

void UUDClientItemViewModel::SetIsHostValue(bool newIsHostValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsHostValue, newIsHostValue);
}

bool UUDClientItemViewModel::GetIsHostValue() const
{
	return IsHostValue;
}