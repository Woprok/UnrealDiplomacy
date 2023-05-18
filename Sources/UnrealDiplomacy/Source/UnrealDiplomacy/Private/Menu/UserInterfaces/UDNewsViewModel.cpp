// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDNewsViewModel.h"

#define LOCTEXT_NAMESPACE "News"

void UUDNewsViewModel::Update()
{
	FString newsTitle = FText(LOCTEXT("News", "Welcome!")).ToString();
	SetNewsTitleText(newsTitle);
	FString message = FText(LOCTEXT("News", "This game is still in active development. You may encounter spooky bugs!")).ToString();
	SetMessageText(message);
	FString close = FText(LOCTEXT("News", "Close")).ToString();
	SetCloseText(close);
}

#undef LOCTEXT_NAMESPACE

void UUDNewsViewModel::SetNewsTitleText(FString newNewsTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NewsTitleText, newNewsTitleText);
}

FString UUDNewsViewModel::GetNewsTitleText() const
{
	return NewsTitleText;
}

void UUDNewsViewModel::SetMessageText(FString newMessageText)
{
	UE_MVVM_SET_PROPERTY_VALUE(MessageText, newMessageText);
}

FString UUDNewsViewModel::GetMessageText() const
{
	return MessageText;
}

void UUDNewsViewModel::SetCloseText(FString newCloseText)
{
	UE_MVVM_SET_PROPERTY_VALUE(CloseText, newCloseText);
}

FString UUDNewsViewModel::GetCloseText() const
{
	return CloseText;
}