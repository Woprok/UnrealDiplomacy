// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDCreditsViewModel.h"

#define LOCTEXT_NAMESPACE "Credits"

FString ConstructCredits()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("Credits", "AUTHORS")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText::FromString(TEXT("Miroslav Valach")).ToString());
	return content.ToString();
}

void UUDCreditsViewModel::Update()
{
	FString creditsTitle = FText(LOCTEXT("Credits", "Credits")).ToString();
	SetCreditsTitleText(creditsTitle);
	FString content = ConstructCredits();
	SetContentText(content);
	FString back = FText(LOCTEXT("Credits", "Back")).ToString();
	SetBackText(back);
}

#undef LOCTEXT_NAMESPACE

void UUDCreditsViewModel::SetCreditsTitleText(FString newCreditsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CreditsTitleText, newCreditsTitleText);
}

FString UUDCreditsViewModel::GetCreditsTitleText() const
{
	return CreditsTitleText;
}

void UUDCreditsViewModel::SetContentText(FString newContentText)
{
	UE_MVVM_SET_PROPERTY_VALUE(ContentText, newContentText);
}

FString UUDCreditsViewModel::GetContentText() const
{
	return ContentText;
}

void UUDCreditsViewModel::SetBackText(FString newBackText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackText, newBackText);
}

FString UUDCreditsViewModel::GetBackText() const
{
	return BackText;
}