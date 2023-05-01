// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "UDEndTurnViewModel.generated.h"

/**
 * End Turn Widget
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDEndTurnViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 Turn = -1;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 Player = -1;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanEndTurn = false;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameInProgress())
		{
			// Set visible

			// Set values
			UpdateTurn(ActionModel->GetCurrentTurnState(), ActionModel->CanEndTurn());
		}
		else
		{
			// Set invisible
		}
	}
	/**
	 * Called by button.
	 */
	UFUNCTION(BlueprintCallable)
	void EndTurn()
	{
		ActionModel->RequestAction(ActionModel->GetAction(UUDSystemActionTurnEnd::ActionTypeId));
	}
protected:
	/**
	 * Called on update to refresh UI elements.
	 */
	void UpdateTurn(FUDTurnInfo turnInfo, bool isAbleToEndTurn)
	{
		SetTurn(turnInfo.Turn);
		SetPlayer(turnInfo.Player);
		SetCanEndTurn(isAbleToEndTurn);
	}
private:
	/**
	 * MVVM Binding.
	 */
	void SetTurn(int32 newTurn)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Turn, newTurn);
	}
	/**
	 * MVVM Binding.
	 */
	void SetPlayer(int32 newPlayer)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(Player, newPlayer);
	}
	/**
	 * MVVM Binding.
	 */
	void SetCanEndTurn(bool newCanEndTurn)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(CanEndTurn, newCanEndTurn);
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetTurn() const
	{
		return Turn;
	}
	/**
	 * MVVM Binding.
	 */
	int32 GetPlayer() const
	{
		return Player;
	}
	/**
	 * MVVM Binding.
	 */
	bool GetCanEndTurn() const
	{
		return CanEndTurn;
	}
};