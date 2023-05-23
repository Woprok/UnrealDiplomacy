// Copyright Miroslav Valach

#include "Menu/UserInterfaces/UDJoinGameViewModel.h"
#include "Menu/UserInterfaces/UDServerItemViewModel.h"
#include "Menu/UDMenuHUD.h"
#include "Core/UDGlobalData.h"
#include "Core/UDGameInstance.h"
#include "Core/UDSessionSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#define LOCTEXT_NAMESPACE "JoinGame"

void UUDJoinGameViewModel::Initialize()
{
	ViewModelType = UUDServerItemViewModel::StaticClass();

	FText joinGameTitle = FText(LOCTEXT("JoinGame", "Join Game"));
	SetJoinGameTitleText(joinGameTitle);
	FText back = FText(LOCTEXT("JoinGame", "Back"));
	SetBackText(back);
	FText refresh = FText(LOCTEXT("JoinGame", "Refresh"));
	SetRefreshText(refresh);

	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->OnFindSessionsCompleteEvent.AddUObject(this, &UUDJoinGameViewModel::OnSessionSearched);
	sessions->OnJoinGameSessionCompleteEvent.AddUObject(this, &UUDJoinGameViewModel::OnSessionJoined);
}

void UUDJoinGameViewModel::Update()
{

}

#undef LOCTEXT_NAMESPACE

void UUDJoinGameViewModel::Back()
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: Back."));
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
	hud->SwitchScreen(hud->MenuScreen);
}

void UUDJoinGameViewModel::Refresh()
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: Refresh."));
	// TODO
	// TEST CODE
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	sessions->CreateSearchSettings();
	sessions->FindSessions();
}

void UUDJoinGameViewModel::OnSessionSearched(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful)
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: Found."));
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());


	TArray<TObjectPtr<UUDViewModel>>& viewModels = hud->GetViewModelCollection(ViewModelCollectionName, ViewModelType, SessionResults.Num());
	InUseViewModelCollection.Empty();
	for (int32 i = 0; i < SessionResults.Num(); i++)
	{
		TObjectPtr<UUDServerItemViewModel> newViewModel = Cast<UUDServerItemViewModel>(viewModels[i]);
		newViewModel->SetContent(SessionResults[i]);
		newViewModel->FullUpdate();
		InUseViewModelCollection.Add(newViewModel);
	}

	//sessions->JoinGameSession(TEXT("JOIN SESSION"), item);
	OnSessionSearchSourceUpdated.Broadcast(InUseViewModelCollection);
}

void UUDJoinGameViewModel::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Log, TEXT("UUDJoinGameViewModel: Join %s."), *LexToString(Result));
	TObjectPtr<UUDSessionSubsystem> sessions = UUDSessionSubsystem::Get(GetWorld());
	if (sessions->TryTravelToCurrentSession(SessionName))
	{
		// TRAVELING
	}
	else
	{
		TObjectPtr<AUDMenuHUD> hud = AUDMenuHUD::Get(GetWorld());
		//hud->ShowNewError();
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