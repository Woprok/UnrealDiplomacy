// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDNationItemViewModel.generated.h"

DECLARE_DELEGATE_OneParam(NationSelectedDelegate, int32);

/**
 * Single nation representation in a list or similiar selectable collection.
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDNationItemViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 NationId = -1;
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
	/**
	 * Changes nation associated with this viewmodel and correctly updates data.
	 */
	UFUNCTION(BlueprintCallable)
	void ChangeDisplayedNation(int32 newNationId)
	{
		SetNationId(newNationId);
		OnUpdate();
	}
	/**
	 * Called from blueprint whenever is this tile selected.
	 */
	UFUNCTION(BlueprintCallable)
	void NotifyNationSelected()
	{
		Selected.ExecuteIfBound(NationId);
	}	
	/**
	 * Invoked when this tile is selected.
	 * Grid is handling what it means to be selected.
	 */
	NationSelectedDelegate Selected;
protected:
	void UpdateNation(FUDNationInfo info)
	{
		SetNationId(info.Id);
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
	int32 GetNationId() const
	{
		return NationId;
	}
};