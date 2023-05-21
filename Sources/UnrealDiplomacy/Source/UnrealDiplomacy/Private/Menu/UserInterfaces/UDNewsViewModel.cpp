// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDNewsViewModel.h"
#include "Menu/UDMenuHUD.h"

#define LOCTEXT_NAMESPACE "News"

FText ConstructNews()
{
	TStringBuilder<128> content;
	content.Append(FText(LOCTEXT("Credits", "You are playing development build.")).ToString());
	content.Append(FText::FromString(TEXT("\n")).ToString());
	content.Append(FText::FromString(TEXT("Most features is missing!")).ToString());
	return FText::FromString(content.ToString());
}

void UUDNewsViewModel::Initialize()
{
	FText newsTitle = FText(LOCTEXT("News", "Welcome!"));
	SetNewsTitleText(newsTitle);
	FText message = ConstructNews();
	SetMessageText(message);
	FText close = FText(LOCTEXT("News", "Close"));
	SetCloseText(close);
}

#undef LOCTEXT_NAMESPACE

void UUDNewsViewModel::Close()
{
	UE_LOG(LogTemp, Log, TEXT("UUDNewsViewModel: Close."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->HideWidget(hud->NewsWidget);
}

void UUDNewsViewModel::SetNewsTitleText(FText newNewsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NewsTitleText, newNewsTitleText);
}

FText UUDNewsViewModel::GetNewsTitleText() const
{
	return NewsTitleText;
}

void UUDNewsViewModel::SetMessageText(FText newMessageText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageText, newMessageText);
}

FText UUDNewsViewModel::GetMessageText() const
{
	return MessageText;
}

void UUDNewsViewModel::SetCloseText(FText newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FText UUDNewsViewModel::GetCloseText() const
{
	return CloseText;
}