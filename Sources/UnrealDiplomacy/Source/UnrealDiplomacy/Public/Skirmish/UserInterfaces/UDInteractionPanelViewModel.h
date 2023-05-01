// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/Actions/UDDealActionCreate.h"
#include "UDInteractionPanelViewModel.generated.h"

/**
 * Contains logic for handling interaction between players such as deals, requests...
 * Handles opening, closing deal window and ensuring that only one deal is present ? (optional but easiest)
 * Handles opening interactions (they might still open automatically), useful for closed interaction window.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDInteractionPanelViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	* MVVM Field.
	*/
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 PendingCount = -1;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// This is visible during the game and shows current info
			UpdateInteractions(ActionModel->GetPendingRequestsCount());
		}
		else
		{
			// This should not be visible ?
		}
	}
	/**
	 * Creates new action and send it to server to create new deal...
	 */
	UFUNCTION(BlueprintCallable)
	void CreateDeal()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDDealActionCreate::ActionTypeId));
	}
protected:
	void UpdateInteractions(int32 count)
	{
		SetPendingCount(count);
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetPendingCount(int32 newPendingCount)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(PendingCount, newPendingCount);
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetPendingCount() const
	{
		return PendingCount;
	}
};