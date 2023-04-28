// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "UDImperialThroneViewModel.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDImperialThroneViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	* MVVM Field.
	*/
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FString CurrentUsurperName;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanTakeThrone;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanAbdicateThrone;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CantUseThrone;
	/**
	 * Fields
	 */
	FUDThroneInfo CurrentUsurper;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// This is visible during the game and shows current info
			UpdateThrone(ActionModel->GetThroneInfo());
		}
		else
		{
			// This should not be visible ?
		}
	}
	/**
	 * Take empty throne.
	 */
	UFUNCTION(BlueprintCallable)
	void TakeThrone()
	{
		ActionModel->RequestAction(ActionModel->GetUsurpThroneAction());
	}
	/**
	 * Giveup throne for nothing in return.
	 */
	UFUNCTION(BlueprintCallable)
	void AbdicateThrone()
	{
		ActionModel->RequestAction(ActionModel->GetAbdicateThroneAction());
	}
protected:
	void UpdateThrone(FUDThroneInfo info)
	{
		CurrentUsurper = info;
		SetCurrentUsurperName(info.UsurperName.ToString());
		if (ActionModel->CanUsurpThrone()) // noone is on throne
		{
			SetCanTakeThrone(true);
			SetCanAbdicateThrone(false);
			SetCantUseThrone(false);
		}
		else if (ActionModel->CanAbdicateThrone()) // you are on throne
		{
			SetCanTakeThrone(false);
			SetCanAbdicateThrone(true);
			SetCantUseThrone(false);
		}
		else // someone else is on throne
		{
			SetCanTakeThrone(false);
			SetCanAbdicateThrone(false);
			SetCantUseThrone(true);
		}
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetCurrentUsurperName(FString newCurrentUsurperName)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CurrentUsurperName, newCurrentUsurperName);
	}
	FString GetCurrentUsurperName() const
	{
		return CurrentUsurperName;
	}
	void SetCanTakeThrone(bool newCanTakeThrone)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CanTakeThrone, newCanTakeThrone);
	}
	bool GetCanTakeThrone() const
	{
		return CanTakeThrone;
	}
	void SetCanAbdicateThrone(bool newCanAbdicateThrone)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CanAbdicateThrone, newCanAbdicateThrone);
	}
	bool GetCanAbdicateThrone() const
	{
		return CanAbdicateThrone;
	}
	void SetCantUseThrone(bool newCantUseThrone)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CantUseThrone, newCantUseThrone);
	}
	bool GetCantUseThrone() const
	{
		return CantUseThrone;
	}
};