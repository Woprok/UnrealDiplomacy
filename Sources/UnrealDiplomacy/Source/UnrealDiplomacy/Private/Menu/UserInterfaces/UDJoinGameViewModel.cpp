// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDJoinGameViewModel.h"
#include "Menu/UserInterfaces/UDServerItemViewModel.h"
#include "Menu/UDMenuHUD.h"
#include "Core/UDGlobalData.h"
#include "Core/UDGameInstance.h"
#include "Core/UDSessionSubsystem.h"
#include "Menu/UDExceptionManagerSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#define LOCTEXT_NAMESPACE "JoinGame"

FText SessionCountToText(int32 sessionCount)
{
	switch (sessionCount)
	{
	case -1:
		return FText(LOCTEXT("JoinGame", "Searching..."));
	case 0:
		return FText(LOCTEXT("JoinGame", "No game found. Refresh again!"));
	case 1:
		return FText(LOCTEXT("JoinGame", "1 game found."));
	default:
		return FText::Format(LOCTEXT("JoinGame", "{0} game found."), sessionCount);
	}
}

void UUDJoinGameViewModel::Setup()
{
	ViewModelType = UUDServerItemViewModel::StaticClass();

	FText joinGameTitle = FText(LOCTEXT("JoinGame", "Join Game"));
	SetJoinGameTitleText(joinGameTitle);
	FText back = FText(LOCTEXT("JoinGame", "Back"));
	SetBackText(back);
	FText refresh = FText(LOCTEXT("JoinGame", "Refresh"));
	SetRefreshText(refresh);
	FText nameHeader = FText(LOCTEXT("JoinGame", "Server Name"));
	SetNameHeaderText(nameHeader);
	FText pingHeader = FText(LOCTEXT("JoinGame", "Ping"));	
	SetPingHeaderText(pingHeader);
	SetSearchText(SessionCountToText(InUseViewModelCollection.Num()));
	FText newIsLANText = FText(LOCTEXT("JoinGame", "Search LAN"));
	SetIsLANText(newIsLANText);

	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->OnFindSessionsCompleteEvent.AddUObject(this, &UUDJoinGameViewModel::OnSessionSearched);
	sessions->OnJoinGameSessionCompleteEvent.AddUObject(this, &UUDJoinGameViewModel::OnSessionJoined);
}

void UUDJoinGameViewModel::Refresh()
{
	RefreshList();
}

FUDDialogueData GetSessionJoinFailed(EOnJoinSessionCompleteResult::Type result)
{
	FString content;
	switch (result)
	{
	case EOnJoinSessionCompleteResult::SessionIsFull:
		content = FText(LOCTEXT("JoinGame", "Session is full.")).ToString();
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		content = FText(LOCTEXT("JoinGame", "Session no longer exists.")).ToString();
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		content = FText(LOCTEXT("JoinGame", "Could not retrieve the session address.")).ToString();
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		content = FText(LOCTEXT("JoinGame", "Already in the session.")).ToString();
		break;
	case EOnJoinSessionCompleteResult::Success:
	case EOnJoinSessionCompleteResult::UnknownError:
	default:
		content = FText(LOCTEXT("JoinGame", "Unknown error occured during the joining session.")).ToString();
		break;
	}
	return FUDDialogueData(
		EUDDialogueContentType::Error,
		FText(LOCTEXT("JoinGame", "Join Game Error")).ToString(),
		content
	);
}

#undef LOCTEXT_NAMESPACE

void UUDJoinGameViewModel::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: Back."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->MenuScreen);
}

void UUDJoinGameViewModel::RefreshList()
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: RefreshList."));
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->CreateSearchSettings(UUDApplicationConverters::FromCheckBoxState(GetIsLANValue()));
	SetSearchText(SessionCountToText(SearchIndicator));
	sessions->FindSessions();
}

void UUDJoinGameViewModel::OnSessionSearched(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: Found."));
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());

	TArray<TObjectPtr<UUDViewModel>> viewModels = hud->GetViewModelCollection(ViewModelCollectionName, ViewModelType, SessionResults.Num());
	InUseViewModelCollection.Empty();
	for (int32 i = 0; i < SessionResults.Num(); i++)
	{
		TObjectPtr<UUDServerItemViewModel> newViewModel = Cast<UUDServerItemViewModel>(viewModels[i]);
		newViewModel->SetContent(SessionResults[i]);
		newViewModel->Refresh();
		InUseViewModelCollection.Add(newViewModel);
	}

	SetSearchText(SessionCountToText(InUseViewModelCollection.Num()));

	SetServerItemList(FUDViewModelList(viewModels));
}

void UUDJoinGameViewModel::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: Join %s."), LexToString(Result));

	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	if (Result == EOnJoinSessionCompleteResult::Success && sessions->TryTravelToCurrentSession(SessionName))
	{
		// On success travel to session level was invoked.
	}
	else
	{
		// Session Join Failed, we can provide error to user.
		TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
		hud->SwitchScreen(hud->JoinGameScreen);
		FUDDialogueData dialogueData = GetSessionJoinFailed(Result);
		UUDExceptionManagerSubsystem::Get(GetWorld())->ShowDialogue(dialogueData);
	}
}

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

void UUDJoinGameViewModel::SetNameHeaderText(FText newNameHeaderText)
{
	UE_MVVM_SET_PROPERTY_VALUE(NameHeaderText, newNameHeaderText);
}

FText UUDJoinGameViewModel::GetNameHeaderText() const
{
	return NameHeaderText;
}

void UUDJoinGameViewModel::SetPingHeaderText(FText newPingHeaderText)
{
	UE_MVVM_SET_PROPERTY_VALUE(PingHeaderText, newPingHeaderText);
}

FText UUDJoinGameViewModel::GetPingHeaderText() const
{
	return PingHeaderText;
}

void UUDJoinGameViewModel::SetSearchText(FText newSearchText)
{
	UE_MVVM_SET_PROPERTY_VALUE(SearchText, newSearchText);
}

FText UUDJoinGameViewModel::GetSearchText() const
{
	return SearchText;
}

void UUDJoinGameViewModel::SetServerItemList(FUDViewModelList newServerItemList)
{
	UE_MVVM_SET_PROPERTY_VALUE(ServerItemList, newServerItemList);
}

FUDViewModelList UUDJoinGameViewModel::GetServerItemList() const
{
	return ServerItemList;
}

void UUDJoinGameViewModel::SetIsLANText(FText newIsLANText)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsLANText, newIsLANText);
}

FText UUDJoinGameViewModel::GetIsLANText() const
{
	return IsLANText;
}

void UUDJoinGameViewModel::SetIsLANValue(ECheckBoxState newIsLANValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(IsLANValue, newIsLANValue);
}

ECheckBoxState UUDJoinGameViewModel::GetIsLANValue() const
{
	return IsLANValue;
}