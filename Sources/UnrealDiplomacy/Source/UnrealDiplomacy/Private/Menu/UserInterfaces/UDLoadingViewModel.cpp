// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDLoadingViewModel.h"

#define LOCTEXT_NAMESPACE "Loading"

FText ConstructGameplayHint()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("Loading", "Did you know this game might be in development hell?")).ToString());
	return FText::FromString(content.ToString());
}

void UUDLoadingViewModel::Initialize()
{
	FText loadingTitle = FText(LOCTEXT("Loading", "Loading..."));
	SetLoadingTitleText(loadingTitle);
	FText gameplayHint = ConstructGameplayHint();
	SetGameplayHintText(gameplayHint);
}

#undef LOCTEXT_NAMESPACE

void UUDLoadingViewModel::SetLoadingTitleText(FText newLoadingTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadingTitleText, newLoadingTitleText);
}

FText UUDLoadingViewModel::GetLoadingTitleText() const
{
	return LoadingTitleText;
}

void UUDLoadingViewModel::SetGameplayHintText(FText newGameplayHintText)
{
	UE_MVVM_SET_PROPERTY_VALUE(GameplayHintText, newGameplayHintText);
}

FText UUDLoadingViewModel::GetGameplayHintText() const
{
	return GameplayHintText;
}