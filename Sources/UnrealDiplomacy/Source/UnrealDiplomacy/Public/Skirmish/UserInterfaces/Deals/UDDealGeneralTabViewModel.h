// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelList.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDDealGeneralTabViewModel.generated.h"

// Forward Declarations

struct FUDDealMinimalInfo;
class UUDParticipantItemViewModel;
class UUDInviteItemViewModel;
class UUDChatViewModel;

/**
 * Window for showing and editing deals.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealGeneralTabViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of main the deal view tab. */
	void InvalidateContent(FUDDealMinimalInfo content);
	/** Set content of main the deal view tab. */
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
	void EndAssemble();
	UFUNCTION()
	void EndDefine();
	UFUNCTION()
	void EndVote();
	UFUNCTION()
	void EndResolution();
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
	FText VoteHelpText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText LeaveText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText CancelText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList ParticipantItemList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelList InviteItemList;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent DealChatContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EndAssembleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EndDefineText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EndVoteText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EndResolutionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ExecuteContractText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText HelpTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText HelpText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanEndAssembleValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanEndDefineValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanEndVoteValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanEndResolutionValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanExecuteContractValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanCloseDealValue;
public:
	virtual void Setup() override;
	UFUNCTION()
	virtual void Refresh() override;
private:
	/** Updates chat. */
	void UpdateChatInstance();
	/** Updates participant list. */
	void UpdateParticipantItemList();
	/** Updates invite list. */
	void UpdateInviteItemList();
	/** Updates execution buttons. */
	void UpdateExecution();
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
	void SetVoteHelpText(FText newVoteHelpText);
	FText GetVoteHelpText() const;
	void SetLeaveText(FText newLeaveText);
	FText GetLeaveText() const;
	void SetCancelText(FText newCancelText);
	FText GetCancelText() const;
	void SetParticipantItemList(FUDViewModelList newParticipantItemList);
	FUDViewModelList GetParticipantItemList() const;
	void SetInviteItemList(FUDViewModelList newInviteItemList);
	FUDViewModelList GetInviteItemList() const;
	void SetDealChatContent(FUDViewModelContent newDealChatContent);
	FUDViewModelContent GetDealChatContent() const;
	void SetExecuteContractText(FText newExecuteContractText);
	FText GetExecuteContractText() const;
	void SetEndResolutionText(FText newEndResolutionText);
	FText GetEndResolutionText() const;
	void SetEndVoteText(FText newEndVoteText);
	FText GetEndVoteText() const;
	void SetEndDefineText(FText newEndDefineText);
	FText GetEndDefineText() const;
	void SetEndAssembleText(FText newEndAssembleText);
	FText GetEndAssembleText() const;
	void SetHelpTitleText(FText newHelpTitleText);
	FText GetHelpTitleText() const;
	void SetHelpText(FText newHelpText);
	FText GetHelpText() const;
	void SetCanExecuteContractValue(bool newCanExecuteContractValue);
	bool GetCanExecuteContractValue() const;
	void SetCanEndResolutionValue(bool newCanEndResolutionValue);
	bool GetCanEndResolutionValue() const;
	void SetCanEndVoteValue(bool newCanEndVoteValue);
	bool GetCanEndVoteValue() const;
	void SetCanEndDefineValue(bool newCanEndDefineValue);
	bool GetCanEndDefineValue() const;
	void SetCanEndAssembleValue(bool newCanEndAssembleValue);
	bool GetCanEndAssembleValue() const;
	void SetCanCloseDealValue(bool newCanCloseDealValue);
	bool GetCanCloseDealValue() const;
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