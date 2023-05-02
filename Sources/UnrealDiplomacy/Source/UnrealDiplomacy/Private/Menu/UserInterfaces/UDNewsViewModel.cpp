// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDNewsViewModel.h"


#define LOCTEXT_NAMESPACE "News"

void UUDNewsViewModel::Update()
{
	auto title = FText(LOCTEXT("News", "Welcome!")).ToString();		
	SetTitle(title);
	auto message = FText(LOCTEXT("News", "This game is still in active development. You may encounter spooky bugs!")).ToString();
	SetMessage(message);
}

#undef LOCTEXT_NAMESPACE

void UUDNewsViewModel::SetTitle(FString newTitle)
{
	UE_MVVM_SET_PROPERTY_VALUE(Title, newTitle);
}

FString UUDNewsViewModel::GetTitle() const
{
	return Title;
}

void UUDNewsViewModel::SetMessage(FString newMessage)
{
	UE_MVVM_SET_PROPERTY_VALUE(Message, newMessage);
}

FString UUDNewsViewModel::GetMessage() const
{
	return Message;
}