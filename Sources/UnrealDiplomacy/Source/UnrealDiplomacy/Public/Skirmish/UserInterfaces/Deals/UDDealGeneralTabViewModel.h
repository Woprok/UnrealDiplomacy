// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealGeneralTabViewModel.generated.h"

// Forward Declarations

struct FUDDealMinimalInfo;
class UUDParticipantItemViewModel;
class UUDInviteItemViewModel;
class UUDChatViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDChatSourceUpdated, const TObjectPtr<UUDChatViewModel>& chatViewModel);

/**
 * Window for showing and editing deals.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealGeneralTabViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the parameter editor.
	 */
	void InvalidateContent(FUDDealMinimalInfo content);
	/**
	 * Set content of the parameter editor.
	 */
	void SetContent(FUDDealMinimalInfo content);
public:
	// Button Functions
	UFUNCTION()
	void ChangeReady();
	UFUNCTION()
	void ChangeVote();
	UFUNCTION()
	void Leave();
	UFUNCTION()
	void Cancel();
	UFUNCTION()
	void CreateContract();
	UFUNCTION()
	void ExecuteContract();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsModeratorValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText StateTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText StateText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ParticipantText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ReadyCountText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText VoteCountText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText InviteableText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ReadyText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText VoteText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LeaveText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CancelText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CreateContractText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ExecuteContractText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList ParticipantItemList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList InviteItemList;
	// Events
	FUDChatSourceUpdated ChatSourceUpdatedEvent;
protected:
	virtual void Initialize() override;
	UFUNCTION()
	virtual void Update() override;
	UFUNCTION()
	void Reload();
private:
	/**
	 * Updates chat.
	 */
	void UpdateChatInstance();
	/**
	 * Updates participant list.
	 */
	void UpdateParticipantItemList();
	/**
	 * Updates invite list.
	 */
	void UpdateInviteItemList();
private:
	// MVVM Setters & Getters
	void SetIsModeratorValue(bool newIsModeratorValue);
	bool GetIsModeratorValue() const;
	void SetStateTitleText(FText newStateTitleText);
	FText GetStateTitleText() const;
	void SetStateText(FText newStateText);
	FText GetStateText() const;
	void SetParticipantText(FText newParticipantText);
	FText GetParticipantText() const;
	void SetReadyCountText(FText newReadyCountText);
	FText GetReadyCountText() const;
	void SetVoteCountText(FText newVoteCountText);
	FText GetVoteCountText() const;
	void SetInviteableText(FText newInviteableText);
	FText GetInviteableText() const;
	void SetReadyText(FText newReadyText);
	FText GetReadyText() const;
	void SetVoteText(FText newVoteText);
	FText GetVoteText() const;
	void SetLeaveText(FText newLeaveText);
	FText GetLeaveText() const;
	void SetCancelText(FText newCancelText);
	FText GetCancelText() const;
	void SetExecuteContractText(FText newExecuteContractText);
	FText GetExecuteContractText() const;
	void SetCreateContractText(FText newCreateContractText);
	FText GetCreateContractText() const;
	void SetParticipantItemList(FUDViewModelList newParticipantItemList);
	FUDViewModelList GetParticipantItemList() const;
	void SetInviteItemList(FUDViewModelList newInviteItemList);
	FUDViewModelList GetInviteItemList() const;
private:
	// Fields
	FUDDealMinimalInfo Content;

	// Current Instances in use...
	FName ParticipantItemViewModelCollectionName = TEXT("ParticipantItemCollection");
	TSubclassOf<UUDViewModel> ParticipantItemViewModelType;
	TArray<TObjectPtr<UUDParticipantItemViewModel>> ParticipantItemViewModelCollection;
	FName InviteItemViewModelCollectionName = TEXT("InviteItemCollection");
	TSubclassOf<UUDViewModel> InviteItemViewModelType;
	TArray<TObjectPtr<UUDInviteItemViewModel>> InviteItemViewModelCollection;
	FName ChatViewModelInstanceName = TEXT("ChatInstance");
	TSubclassOf<UUDViewModel> ChatViewModelType;
	TObjectPtr<UUDChatViewModel> ChatViewModelInstance;
};