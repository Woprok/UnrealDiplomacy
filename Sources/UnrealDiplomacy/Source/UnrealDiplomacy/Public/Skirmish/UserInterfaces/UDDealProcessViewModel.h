// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "UDDealProcessViewModel.generated.h"

#define LOCTEXT_NAMESPACE "ActionUI"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUDParticipantsUpdated, FUDDealParticipantsInfo, infos);

UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDParticipantViewModel : public UUDStaticViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString Name;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsParticipant;
private:
	FUDPlayerInfo CurrentInfo;
	int32 CurrentDealUniqueId;
public:
	UFUNCTION(BlueprintCallable)
	void SetBindingTarget(FUDPlayerInfo info, int32 dealUniqueId)
	{
		CurrentInfo = info;
		CurrentDealUniqueId = dealUniqueId;
		auto rs1 = FText::Format(
			LOCTEXT("Participant", "Player {0}"),
			info.Id
		).ToString();
		SetName(rs1);
		SetIsParticipant(ActionModel->IsParticipantInCurrentDeal(dealUniqueId, info.Id));
	}

	UFUNCTION(BlueprintCallable)
	void InvitePlayer()
	{
		ActionModel->RequestAction(ActionModel->GetInviteParticipantDealAction(CurrentDealUniqueId, CurrentInfo.Id));
	}
	UFUNCTION(BlueprintCallable)
	void LeaveDeal()
	{
		ActionModel->RequestAction(ActionModel->GetLeaveParticipantDealAction(CurrentDealUniqueId, CurrentInfo.Id));
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetName(FString newName)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Name, newName);
	}
	/**
	 * MVVM Binding.
	 */
	void SetIsParticipant(bool newIsParticipant)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsParticipant, newIsParticipant);
	}
	/**
	 * MVVM Binding.
	 */
	FString GetName() const
	{
		return Name;
	}
	/**
	 * MVVM Binding.
	 */
	bool GetIsParticipant() const
	{
		return IsParticipant;
	}
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDDealProcessViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
  	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString SessionDescription;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsSessionActive;

public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			FUDDealInfo info = ActionModel->GetDealInfoAnyDEBUG();
			if (info.Result >= EUDDealSimulationResult::Opened)
			{
				OnDealPresent(info);
			}
			else
			{
				OnDealEmpty(info);
			}
		}
		else
		{
			// This should not be visible ?
		}
	}
	/**
	 * Invoked when this requires view to rebind.
	 */
	UPROPERTY(BlueprintAssignable)
	FUDParticipantsUpdated ParticipantsOnUpdated;
protected:
	void OnDealEmpty(FUDDealInfo info)
	{
		SetIsSessionActive(false);
		auto rs1 = FText(
			LOCTEXT("DealState", "No deal is currently discussed with you.")
		).ToString();
		SetSessionDescription(rs1);
	}
	void OnDealPresent(FUDDealInfo info)
	{
		SetIsSessionActive(true);
		auto rs1 = FText(
			LOCTEXT("DealState", "Deal is currently discussed with you.")
		).ToString();
		SetSessionDescription(rs1);

		auto data = ActionModel->GetDealParticipants(info.DealUniqueId);
		ParticipantsOnUpdated.Broadcast(data);

	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetSessionDescription(FString newSessionDescription)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(SessionDescription, newSessionDescription);
	}
	/**
	 * MVVM Binding.
	 */
	void SetIsSessionActive(bool newIsSessionActive)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsSessionActive, newIsSessionActive);
	}
	/**
	 * MVVM Binding.
	 */
	FString GetSessionDescription() const
	{
		return SessionDescription;
	}	
	/**
	 * MVVM Binding.
	 */
	bool GetIsSessionActive() const
	{
		return IsSessionActive;
	}
};
#undef LOCTEXT_NAMESPACE