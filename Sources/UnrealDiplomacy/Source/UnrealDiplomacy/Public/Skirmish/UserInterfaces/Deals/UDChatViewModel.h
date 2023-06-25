// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDChatViewModel.generated.h"

// Forward Declarations

enum ETextCommit::Type : int;
struct FUDDealMinimalInfo;
class UUDChatItemViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDChatSourceUpdated, const TArray<TObjectPtr<UUDChatItemViewModel>>& chatItemViewModels);

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDChatViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the chat.
	 */
	void SetContent(FUDDealMinimalInfo content);
public:
	// Button Functions
	UFUNCTION()
	void StartTextEditation(const FText& InText);
	UFUNCTION()
	void StopTextEditation(const FText& InText, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void Send();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ChatTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SendText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SelectedText;
	// Events
	FUDChatSourceUpdated ChatSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/**
	 * Updates chat history list.
	 */
	void UpdateChatItemList();
private:
	// MVVM Setters & Getters
	void SetChatTitleText(FText newChatTitleText);
	FText GetChatTitleText() const;
	void SetSendText(FText newSendText);
	FText GetSendText() const;
	void SetSelectedText(FText newSelectedText);
	FText GetSelectedText() const;
private:
	// Fields
	bool IsTextEditing = false;
	FUDDealMinimalInfo Content;

	FName ChatItemViewModelCollectionName = TEXT("ChatItemCollection");
	TSubclassOf<UUDViewModel> ChatItemViewModelType;
	TArray<TObjectPtr<UUDChatItemViewModel>> ChatItemViewModelCollection;
};