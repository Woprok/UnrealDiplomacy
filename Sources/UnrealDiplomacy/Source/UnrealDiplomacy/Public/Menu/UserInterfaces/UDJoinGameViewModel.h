// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDJoinGameViewModel.generated.h"

// Forward Declarations

namespace EOnJoinSessionCompleteResult
{
	enum Type;
}
class FOnlineSessionSearchResult;
class UUDServerItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDSearchSourceUpdated, const TArray<TObjectPtr<UUDServerItemViewModel>>& serverItemViewModels);

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
	void Refresh();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText JoinGameTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText BackText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText RefreshText;
	// Events
	FUDSearchSourceUpdated OnSessionSearchSourceUpdated;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Callback for session search attempt.
	 */
	void OnSessionSearched(const TArray<FOnlineSessionSearchResult>& SessionResults, bool Successful);
	/**
	 * Callback for session joined.
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
private:
	// Fields
	FName ViewModelCollectionName = TEXT("JoinServerItemCollection");
	TSubclassOf<UUDViewModel> ViewModelType;
	TArray<TObjectPtr<UUDServerItemViewModel>> InUseViewModelCollection;
};