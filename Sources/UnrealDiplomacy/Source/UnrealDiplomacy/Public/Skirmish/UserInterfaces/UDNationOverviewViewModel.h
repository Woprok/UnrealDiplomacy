// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDNationOverviewViewModel.generated.h"

/**
 * Nation Iverview/Management Widget
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDNationOverviewViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 NationId = -1;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 Gold = -1;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// This is visible during the game and shows current info
			UpdateNation(ActionModel->GetCurrentNationState(NationId));
		}
		else
		{
			// This should not be visible ?
		}
	}
	UFUNCTION(BlueprintCallable)
	void SetSelectedNation(int32 newNationId)
	{
		SetNationId(newNationId);
		OnUpdate();
	}
protected:
	void UpdateNation(FUDNationInfo info)
	{
		SetNationId(info.Id);
		//SetCanTakeTile(isOwnedByWorld);
		SetGold(info.Gold);
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetNationId(int32 newNationId)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(NationId, newNationId);
	}
	/**
	 * MVVM Binding.
	 */
	void SetGold(int32 newGold)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Gold, newGold);
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetNationId() const
	{
		return NationId;
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetGold() const
	{
		return Gold;
	}
};