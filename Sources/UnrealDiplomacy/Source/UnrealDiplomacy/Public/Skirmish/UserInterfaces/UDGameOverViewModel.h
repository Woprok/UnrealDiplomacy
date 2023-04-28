// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelBase.h"
#include "UDGameOverViewModel.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameOverViewModel : public UUDViewModelBase
{
	GENERATED_BODY()
public:
	/**
	* MVVM Field.
	*/
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsGameOver;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsWinner;
	/**
	 * Fields
	 */
	FUDGameStateInfo CurrentInfo;
public:
	virtual void OnUpdate() override
	{
		if (ActionModel->IsGameFinished())
		{
			// This is visible during the game and shows current info
			UpdateGameInfo(ActionModel->GetGameStateInfo());
		}
		else
		{
			// This should not be visible ?
		}
	}
protected:
	void UpdateGameInfo(FUDGameStateInfo info)
	{
		CurrentInfo = info;
		SetIsGameOver(info.IsGameFinished);
		SetIsWinner(ActionModel->IsLocalPlayerWinner());
	}
private:
	void SetIsGameOver(bool newIsGameOver)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsGameOver, newIsGameOver);
	}
	bool GetIsGameOver() const
	{
		return IsGameOver;
	}
	void SetIsWinner(bool newIsWinner)
	{
		// Set checks if value changed.
		UE_MVVM_SET_PROPERTY_VALUE(IsWinner, newIsWinner);
	}
	bool GetIsWinner() const
	{
		return IsWinner;
	}
};