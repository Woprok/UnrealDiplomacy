// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDJoinGameViewModel.h"

#define LOCTEXT_NAMESPACE "JoinGame"

void UUDJoinGameViewModel::Update()
{
	FText joinGameTitle = FText(LOCTEXT("JoinGame", "Join Game"));
	SetJoinGameTitleText(joinGameTitle);
	FText back = FText(LOCTEXT("JoinGame", "Back"));
	SetBackText(back);
	FText refresh = FText(LOCTEXT("JoinGame", "Refresh"));
	SetRefreshText(refresh);
}

#undef LOCTEXT_NAMESPACE

void UUDJoinGameViewModel::SetJoinGameTitleText(FText newJoinGameTitleText)
{
	UE_MVVM_SET_PROPERTY_VALUE(JoinGameTitleText, newJoinGameTitleText);
}

FText UUDJoinGameViewModel::GetJoinGameTitleText() const
{
	return JoinGameTitleText;
}

void UUDJoinGameViewModel::SetBackText(FText newBackText)
{
	UE_MVVM_SET_PROPERTY_VALUE(BackText, newBackText);
}

FText UUDJoinGameViewModel::GetBackText() const
{
	return BackText;
}

void UUDJoinGameViewModel::SetRefreshText(FText newRefreshText)
{
	UE_MVVM_SET_PROPERTY_VALUE(RefreshText, newRefreshText);
}

FText UUDJoinGameViewModel::GetRefreshText() const
{
	return RefreshText;
}