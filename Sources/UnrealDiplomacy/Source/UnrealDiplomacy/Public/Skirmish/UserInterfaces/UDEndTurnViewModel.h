// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Core/Simulation/UDActionAdministrator.h"
#include "UDEndTurnViewModel.generated.h"

/**
 * TODO check if someone found or implemented function binding to events
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALDIPLOMACY_API UUDEndTurnViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 Turn;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	int32 Player;
	/**
	 * MVVM Field.
	 */
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanEndTurn;
public:
	/**
	 * Sets reference to Model (this tries follow principle MVVM[ActionAdministrator<->ViewModel<->View])
	 */
	UFUNCTION(BlueprintCallable)
	void SetAuthority(UUDActionAdministrator* model)
	{
		ActionModel = model;
	}
	/**
	 * Public handle for notifying view model to update it's binded values from model.
	 */
	UFUNCTION(BlueprintCallable)
	void OnUpdate()
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
		ActionModel->RequestAction(ActionModel->GetEndTurnAction());
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
	UPROPERTY()
	TObjectPtr<UUDActionAdministrator> ActionModel = nullptr;
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