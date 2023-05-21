// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDCreditsViewModel.h"

#define LOCTEXT_NAMESPACE "Credits"

FText ConstructCredits()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("Credits", "AUTHORS")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText::FromString(TEXT("Miroslav Valach")).ToString());
	return FText::FromString(content.ToString());
}

void UUDCreditsViewModel::Update()
{
	FText creditsTitle = FText(LOCTEXT("Credits", "Credits"));
	SetCreditsTitleText(creditsTitle);
	FText content = ConstructCredits();
	SetContentText(content);
	FText back = FText(LOCTEXT("Credits", "Back"));
	SetBackText(back);
}

#undef LOCTEXT_NAMESPACE

void UUDCreditsViewModel::SetCreditsTitleText(FText newCreditsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreditsTitleText, newCreditsTitleText);
}

FText UUDCreditsViewModel::GetCreditsTitleText() const
{
	return CreditsTitleText;
}

void UUDCreditsViewModel::SetContentText(FText newContentText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ContentText, newContentText);
}

FText UUDCreditsViewModel::GetContentText() const
{
	return ContentText;
}

void UUDCreditsViewModel::SetBackText(FText newBackText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackText, newBackText);
}

FText UUDCreditsViewModel::GetBackText() const
{
	return BackText;
}