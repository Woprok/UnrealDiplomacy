// Copyright Miroslav Valach

#include "Lobby/UserInterfaces/UDStrategyOptionViewModel.h"

#define LOCTEXT_NAMESPACE "StrategyOption"

void UUDStrategyOptionViewModel::Initialize()
{
	FText optionToolTip = FText(LOCTEXT("StrategyOption", "ToolTip"));
	SetOptionToolTipText(optionToolTip);
}

void UUDStrategyOptionViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDStrategyOptionViewModel::SetContent(UObject* content)
{
	Content = content;
}

const UObject* UUDStrategyOptionViewModel::GetContent() const
{
	return Content;
}

void UUDStrategyOptionViewModel::SetOptionToolTipText(FText newOptionToolTipText)
{
	UE_MVVM_SET_PROPERTY_VALUE(OptionToolTipText, newOptionToolTipText);
}

FText UUDStrategyOptionViewModel::GetOptionToolTipText() const
{
	return OptionToolTipText;
}