// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "UDJoinGameViewModel.generated.h"

// Forward Declarations

namespace EOnJoinSessionCompleteResult
{
	enum Type;
}
class FOnlineSessionSearchResult;
class UUDServerItemViewModel;

/**
 * ViewModel for joining game.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDJoinGameViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	// Button Functions
	UFUNCTION()
	void Back();
	UFUNCTION()
	void RefreshList();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText JoinGameTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RefreshText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameHeaderText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PingHeaderText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SearchText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList ServerItemList;
	// Events
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Callback for session search attempt.
	 * Finishes the search process by populating UI again.
	 */
	void OnSessionSearched(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
	/**
	 * Callback for session joined.
	 * Finishes the join process.
	 */
	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
private:
	// MVVM Setters & Getters
	void SetJoinGameTitleText(FText newCreateGameTitleText);
	FText GetJoinGameTitleText() const;
	void SetBackText(FText newBackText);
	FText GetBackText() const;
	void SetRefreshText(FText newRefreshText);
	FText GetRefreshText() const;
	void SetNameHeaderText(FText newNameHeaderText);
	FText GetNameHeaderText() const;
	void SetPingHeaderText(FText newPingHeaderText);
	FText GetPingHeaderText() const;
	void SetSearchText(FText newSearchText);
	FText GetSearchText() const;
	void SetServerItemList(FUDViewModelList newServerItemList);
	FUDViewModelList GetServerItemList() const;
private:
	// Fields
	FName ViewModelCollectionName = TEXT("JoinServerItemCollection");
	TSubclassOf<UUDViewModel> ViewModelType;
	TArray<TObjectPtr<UUDServerItemViewModel>> InUseViewModelCollection;
	const int32 SearchIndicator = -1;
};